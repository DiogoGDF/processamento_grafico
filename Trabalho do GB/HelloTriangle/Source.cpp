// Aluno: Diogo Garbinato de Fagundes		Matrícula: 1189650

#include <iostream>
#include <string>
#include <assert.h>
#include <windows.h>  
#include <fstream>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Shader.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "Sprite.h"

#include "Timer.h"

enum directions { NONE = -1, LEFT, RIGHT, UP, DOWN };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint loadTexture(string filePath, int& imgWidth, int& imgHeight);
bool CheckCollision(Sprite& one, Sprite& two);

const GLuint WIDTH = 800, HEIGHT = 600;

int dir = NONE;

GLuint tilesetTexID;
glm::vec2 offsetTex; 
GLuint VAOTile;
int nTiles;
glm::vec2 tileSize;

glm::vec2 tilemapSize;
const int MAX_COLUNAS = 15;
const int MAX_LINHAS = 15;
int tilemap[MAX_LINHAS][MAX_COLUNAS]; 

void loadMap(string fileName);
GLuint setupTile();
void drawMap(Shader& shader);

glm::vec2 iPos; 

glm::vec2 posIni; 

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	Shader shader("../shaders/HelloTriangle.vs", "../shaders/HelloTriangle.fs");
	Shader shaderDebug("../shaders/HelloTriangle.vs", "../shaders/HelloTriangleDebug.fs");

	int imgWidth, imgHeight;
	GLuint texID = loadTexture("./knight.png", imgWidth, imgHeight);

	Sprite player, coin, sprite3, background, ground, sprite4;
	player.inicializar(texID, 1, 1, glm::vec3(400.0, 150.0, 0.0), glm::vec3(imgWidth * 0.5, imgHeight * 0.5, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	player.setShader(&shader);
	player.setShaderDebug(&shaderDebug);

	//texID = loadTexture("../Textures/items/Pirate-Stuff/Icon31.png", imgWidth, imgHeight);
	coin.inicializar(texID, 1, 1, glm::vec3(450.0, 700.0, 0.0), glm::vec3(imgWidth * 2, imgHeight * 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	coin.setShader(&shader);
	coin.setShaderDebug(&shaderDebug);

	//sprite2.inicializar(glm::vec3(200.0,300.0,0.0), glm::vec3(100.0,50.0,1.0));
	//sprite2.setShader(&shader);

	//sprite3.inicializar(glm::vec3(600.0,300.0,0.0), glm::vec3(50.0,100.0,1.0), 0.0,glm::vec3(0.0,0.5,0.0));
	//sprite3.setShader(&shader);
	//GLuint texID2 = loadTexture("../Textures/backgrounds/fantasy-set/PNG/Battleground1/Bright/Battleground1.png", imgWidth, imgHeight);

	//background.inicializar(texID2, 1, 1, glm::vec3(400.0, 300.0, 0.0), glm::vec3(imgWidth / 2, imgHeight / 2, 1.0), 0.0, glm::vec3(0.0, 1.0, 1.0));
	background.setShader(&shader);
	background.setShaderDebug(&shaderDebug);

	//ground.inicializar(glm::vec3(400.0,100.0,0.0), glm::vec3(800.0,200.0,1.0),0.0,glm::vec3(0.5,0.5,0.0));
	//ground.setShader(&shader);

	//sprite4.inicializar(glm::vec3(200.0,500.0,0.0), glm::vec3(100.0,100.0,1.0),45.0,glm::vec3(1.0,1.0,0.0));
	//sprite4.setShader(&shader);

	//glm::mat4 projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

	loadMap("map2.txt");
	VAOTile = setupTile();

	shader.Use();

	glm::mat4 projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
	shader.setMat4("projection", glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texBuffer", 0);

	shaderDebug.Use();
	shaderDebug.setMat4("projection", glm::value_ptr(projection));

	posIni.x = tileSize.x / 2;
	posIni.y = tileSize.y / 2;

	iPos.x = 1;
	iPos.y = 1;

	while (!glfwWindowShouldClose(window))
	{
		//timer.start();
		glfwPollEvents();



		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawMap(shader);
		float x = posIni.x + iPos.x * tileSize.x;
		float y = posIni.y + iPos.y * tileSize.y;
		player.setPosicao(glm::vec3(x, y, 0.0)); 
		player.desenhar();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		dir = LEFT;
		iPos.x -= 1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		dir = RIGHT;
		iPos.x += 1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		dir = LEFT;
		iPos.y -= 1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		dir = RIGHT;
		iPos.y += 1;
	}

	if (action == GLFW_RELEASE)
	{
		dir = NONE;
	}

}

GLuint loadTexture(string filePath, int& imgWidth, int& imgHeight)
{
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		imgWidth = width;
		imgHeight = height;

		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	return texID;
}

bool CheckCollision(Sprite& one, Sprite& two)
{
	// collision x-axis?
	bool collisionX = one.getPMax().x >= two.getPMin().x &&
		two.getPMax().x >= one.getPMin().x;
	// collision y-axis?
	bool collisionY = one.getPMax().y >= two.getPMin().y &&
		two.getPMax().y >= one.getPMin().y;
	// collision only if on both axes
	return collisionX && collisionY;
}

void loadMap(string fileName)
{
	ifstream arqEntrada;
	arqEntrada.open(fileName); 
	if (arqEntrada)
	{
		string textureName;
		int width, height;
		arqEntrada >> textureName >> nTiles >> tileSize.y >> tileSize.x;
		tilesetTexID = loadTexture(textureName, width, height);
		cout << textureName << " " << nTiles << " " << tileSize.y << " " << tileSize.x << endl;
		arqEntrada >> tilemapSize.y >> tilemapSize.x; 
		cout << tilemapSize.y << " " << tilemapSize.x << endl;
		for (int i = 0; i < tilemapSize.y; i++)
		{
			for (int j = 0; j < tilemapSize.x; j++) 
			{
				arqEntrada >> tilemap[i][j];
				cout << tilemap[i][j] << " ";
			}
			cout << endl;
		}

	}
	else
	{
		cout << "Houve um problema na leitura de " << fileName << endl;
	}
}

GLuint setupTile()
{
	GLuint VAO;

	offsetTex.s = 1.0 / (float)nTiles;
	offsetTex.t = 1.0;
	glm::vec3 cor;
	cor.r = 1.0;
	cor.g = 0.0;
	cor.b = 1.0;
	GLfloat vertices[] = {
		//x   y    z    r      g      b      s    t
		-0.5, 0.5, 0.0, cor.r, cor.g, cor.b, 0.0, offsetTex.t, //v0
		-0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
		 0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t, //v2
		-0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
		 0.5,-0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, 0.0, //v3
		 0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t  //v2
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}

void drawMap(Shader& shader)
{
	shader.Use();

	glBindTexture(GL_TEXTURE_2D, tilesetTexID);
	glBindVertexArray(VAOTile);

	for (int i = 0; i < tilemapSize.y; i++)
	{
		for (int j = 0; j < tilemapSize.x; j++)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(posIni.x + j * tileSize.x, posIni.y + i * tileSize.y, 0.0));
			model = glm::scale(model, glm::vec3(tileSize.x, tileSize.y, 1.0));
			shader.setMat4("model", glm::value_ptr(model));

			int indiceTile = tilemap[i][j];
			shader.setVec2("offsetTex", indiceTile * offsetTex.x, offsetTex.y);

			glDrawArrays(GL_TRIANGLES, 0, 6);

		}

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
