// Aluno: Diogo Garbinato de Fagundes		Matrícula: 1189650

#include <iostream>
#include <string>
#include <assert.h>
#include <windows.h>  

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//Classe que gerencia os shaders
#include "Shader.h"

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB_IMAGE
#include <stb_image.h>

#include "Sprite.h"
#include "Tile.h"

#include "Timer.h"

enum directions { NONE = -1, LEFT, RIGHT, UP, DOWN };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
GLuint loadTexture(string filePath, int& imgWidth, int& imgHeight);
bool CheckCollision(Sprite& one, Sprite& two);

const GLuint WIDTH = 800, HEIGHT = 600;

int dir = NONE;

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



	// Compilando e buildando o programa de shader
	Shader shader("../shaders/HelloTriangle.vs", "../shaders/HelloTriangle.fs");
	Shader shaderDebug("../shaders/HelloTriangle.vs", "../shaders/HelloTriangleDebug.fs");

	int imgWidth, imgHeight;
	GLuint texID = loadTexture("../../Textures/characters/PNG/Knight/walk.png", imgWidth, imgHeight);

	//Criação de um objeto Sprite
	Sprite player, coin, sprite3, background, ground, sprite4;
	player.inicializar(texID, 1, 6, glm::vec3(400.0, 150.0, 0.0), glm::vec3(imgWidth * 2, imgHeight * 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	player.setShader(&shader);
	player.setShaderDebug(&shaderDebug);

	texID = loadTexture("../../Textures/items/Pirate-Stuff/Icon31.png", imgWidth, imgHeight);
	//Criação de um objeto Sprite
	coin.inicializar(texID, 1, 1, glm::vec3(450.0, 700.0, 0.0), glm::vec3(imgWidth * 2, imgHeight * 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	coin.setShader(&shader);
	coin.setShaderDebug(&shaderDebug);

	GLuint texID2 = loadTexture("../../Textures/backgrounds/fantasy-set/PNG/Battleground1/Bright/Battleground1.png", imgWidth, imgHeight);
	background.inicializar(texID2, 1, 1, glm::vec3(400.0, 300.0, 0.0), glm::vec3(imgWidth / 2, imgHeight / 2, 1.0), 0.0, glm::vec3(0.0, 1.0, 1.0));
	background.setShader(&shader);
	background.setShaderDebug(&shaderDebug);

	Tile tile;
	tile.inicializar(texID2, 1, 1, glm::vec3(400.0, 300.0, 0.0), glm::vec3(64, 32, 1.0), 0.0, glm::vec3(0.0, 1.0, 1.0));
	tile.setShader(&shader);
	tile.setShaderDebug(&shaderDebug);


	//Matriz de projeção (paralela ortográfica)
	// Exercício 1 da Lista 2
	//glm::mat4 projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);


	//Habilita o shader que sera usado (glUseProgram)
	shader.Use();


	// Exercício 2 da Lista 2
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	//Enviando para o shader via variável do tipo uniform (glUniform....)
	shader.setMat4("projection", glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texBuffer", 0);

	shaderDebug.Use();
	shaderDebug.setMat4("projection", glm::value_ptr(projection));

	//Timer timer;


	while (!glfwWindowShouldClose(window))
	{
		//timer.start();
		glfwPollEvents();

		if (dir == LEFT)
		{
			player.moveLeft();
		}
		else if (dir == RIGHT)
		{
			player.moveRight();
		}

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//background.desenhar();
		//ground.desenhar();
		//player.desenhar();
		//coin.moveItem();
		//coin.desenhar();
		tile.desenhar();

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_A)
	{
		dir = LEFT;
	}
	if (key == GLFW_KEY_D)
	{
		dir = RIGHT;
	}

	if (action == GLFW_RELEASE)
	{
		dir = NONE;
	}

}

GLuint loadTexture(string filePath, int& imgWidth, int& imgHeight)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
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
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
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
		std::cout << "Failed to load texture " << filePath << std::endl;
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
