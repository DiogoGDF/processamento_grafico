// Aluno: Diogo Garbinato de Fagundes		Matrícula: 1189650

// Importações
#include <iostream>
#include <string>
#include <assert.h>
#include <windows.h>  
#include <fstream>
#include <vector>

using namespace std;

// GLAD e GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM (matemática com opengl)
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Trilha sonora
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#pragma comment(lib, "winmm.lib")

// Biblioteca para carregar imagens
#include <stb_image.h>

// Classes
#include "Sprite.h"
#include "Shader.h"

// Enum para direções
enum directions { NONE = -1, LEFT, RIGHT, UP, DOWN };

// Função de callback para teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Função para carregar textura
GLuint loadTexture(string filePath, int& imgWidth, int& imgHeight);
// Função para verificar colisão
bool CheckCollision(Sprite& one, Sprite& two);

// Funções para cálculos de posição
glm::vec2 calculoPosicaoDesenho(int column, int row, float tileWidth, float tileHeight);
glm::vec2 calcularPosicaoTile(glm::vec2 posInicial, glm::vec2 posDesenho, glm::vec2 tileSize, float rotacao);

// Dimensões da janela
const GLuint WIDTH = 1200, HEIGHT = 1000;

// Variáveis globais
int dir = NONE;
GLuint tilesetTexID;
glm::vec2 offsetTex; 
GLuint VAOTile;
int nTiles;
glm::vec2 tileSize;
glm::vec2 tilemapSize;

// Matriz para armazenar o mapa
const int MAX_COLUNAS = 20;
const int MAX_LINHAS = 20;
int tilemap[MAX_LINHAS][MAX_COLUNAS]; 

// Funções para carregar e desenhar o mapa
void loadMap(string fileName);
GLuint setupTile();
void drawMap(Shader& shader);

// Mais variáveis globais
glm::vec2 iPos; 
glm::vec2 posIni; 
Sprite idle, running;
bool moving = false;
int score = 0;

int main()
{
	// Trilha sonora
	PlaySound(TEXT("../../Soundtrack/end_of_line.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	glfwInit();

	// Versão do OpenGl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Criação da janela
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Inicialização do GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Compilando shaders
	Shader shader("../shaders/HelloTriangle.vs", "../shaders/HelloTriangle.fs");
	Shader shaderDebug("../shaders/HelloTriangle.vs", "../shaders/HelloTriangleDebug.fs");

	// ************* Texturas e Sprites *************
	int imgWidth, imgHeight;
	Sprite chip1, chip2, chip3;

	// Personagem parado
	GLuint texID = loadTexture("../../Textures/characters/PNG/Biker/Biker_idle.png", imgWidth, imgHeight);
	idle.inicializar(texID, 1, 4, glm::vec3(400.0, 60.0, 0.0), glm::vec3(imgWidth, imgHeight, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	idle.setShader(&shader);
	idle.setShaderDebug(&shaderDebug);

	// Personagem correndo
	texID = loadTexture("../../Textures/characters/PNG/Biker/Biker_run.png", imgWidth, imgHeight);
	running.inicializar(texID, 1, 6, glm::vec3(400.0, 60.0, 0.0), glm::vec3(imgWidth, imgHeight, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	running.setShader(&shader);
	running.setShaderDebug(&shaderDebug);

	// Chips
	texID = loadTexture("../../Textures/items/Cyberpunk/microchip.png", imgWidth, imgHeight);
	chip1.inicializar(texID, 1, 1, glm::vec3(400.0, 90.0, 0.0), glm::vec3(imgWidth / 2, imgHeight / 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	chip1.setShader(&shader);
	chip1.setShaderDebug(&shaderDebug);

	chip2.inicializar(texID, 1, 1, glm::vec3(200.0, 220.0, 0.0), glm::vec3(imgWidth / 2, imgHeight / 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	chip2.setShader(&shader);
	chip2.setShaderDebug(&shaderDebug);

	chip3.inicializar(texID, 1, 1, glm::vec3(600.0, 220.0, 0.0), glm::vec3(imgWidth / 2, imgHeight / 2, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0));
	chip3.setShader(&shader);
	chip3.setShaderDebug(&shaderDebug);

	// Carrega o mapa
	loadMap("map.txt");
	VAOTile = setupTile();

	shader.Use();

	// Matriz de projeção
	glm::mat4 projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
	shader.setMat4("projection", glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texBuffer", 0);

	shaderDebug.Use();
	shaderDebug.setMat4("projection", glm::value_ptr(projection));

	// Posição inicial do mapa
	posIni.x = tileSize.x * 1.2;
	posIni.y = tileSize.y * 12;

	// Posição inicial do jogador
	iPos.x = 10;
	iPos.y = 3;

	// Loop principal
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Desenha o mapa
		drawMap(shader);

		float x = posIni.x + iPos.x * tileSize.x;
		float y = posIni.y + iPos.y * tileSize.y;

		// Verifica colisão com os chips
		std::vector<Sprite*> chips = { &chip1, &chip2, &chip3 };
		for (auto& chip : chips) {
			if (CheckCollision(idle, *chip) && chip->ativo) {
				chip->ativo = false; // Marca o chip como coletado
				score++; // Incrementa a pontuação, assumindo que você queira rastrear isso
			}
		}
		
		// Desenha apenas os chips ativos
		for (auto& chip : chips) {
			if (chip->ativo) {
				chip->desenhar();
			}
		}

		// Desenha o jogador de acordo com o estado
		if (moving)
		{
			running.setPosicao(glm::vec3(x, y, 0.0));
			running.desenhar();
		}
		else
		{
			idle.setPosicao(glm::vec3(x, y, 0.0));
			idle.desenhar();
		}
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Função de callback para teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// Terminar o programa com esc
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	glm::vec2 novaPos = iPos;
	bool podeMover = true;

	// Movimentação básica do jogador
	if (key == GLFW_KEY_Q) { // NO
		novaPos.x -= 0.1;
		novaPos.y -= 0.1;
	}
	else if (key == GLFW_KEY_W) { // N
		novaPos.y -= 0.1;
	}
	else if (key == GLFW_KEY_E) { // NE
		novaPos.x += 0.1;
		novaPos.y -= 0.1;
	}
	else if (key == GLFW_KEY_D) { // L
		novaPos.x += 0.1;
	}
	else if (key == GLFW_KEY_A) { // O
		novaPos.x -= 0.1;
	}
	else if (key == GLFW_KEY_LEFT_SHIFT) { // SO
		novaPos.x -= 0.1;
		novaPos.y += 0.1;
	}
	else if (key == GLFW_KEY_S) { // S
		novaPos.y += 0.1;
	}
	else if (key == GLFW_KEY_C) { // SE
		novaPos.x += 0.1;
		novaPos.y += 0.1;
	}

	// Verificação de colisão com tiles de gelo (número 4) NÃO ESTÁ FUNCIONAL
	for (int i = 0; i < tilemapSize.y; i++) {
		for (int j = 0; j < tilemapSize.x; j++) {
			if (tilemap[i][j] == 4) {
				glm::vec2 posDesenho = calculoPosicaoDesenho(j, i, tileSize.x, tileSize.y);
				glm::vec2 posTile = calcularPosicaoTile(posIni, posDesenho, tileSize, 0.0f);

				// Verifica se a nova posição do jogador colide com o tile de gelo
				if (novaPos.x > posTile.x - tileSize.x / 2 && novaPos.x < posTile.x + tileSize.x / 2 &&
					novaPos.y > posTile.y - tileSize.y / 2 && novaPos.y < posTile.y + tileSize.y / 2) {
					podeMover = false;
					break;
				}
			}
		}
		if (!podeMover) break;
	}

	// Animação
	if (podeMover) {
		if (key == GLFW_KEY_Q || key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_LEFT_SHIFT) {
			dir = LEFT;
			running.moveLeft();
			moving = true;
		}
		else if (key == GLFW_KEY_E || key == GLFW_KEY_D || key == GLFW_KEY_S || key == GLFW_KEY_C) {
			dir = RIGHT;
			running.moveRight();
			moving = true;
		}

		if (action == GLFW_RELEASE) {
			moving = false;
			idle.setPosicao(running.getPosicao());
			if (dir == LEFT) {
				idle.moveLeft();
			}
			else if (dir == RIGHT) {
				idle.moveRight();
			}
			dir = NONE;
		}

		iPos = novaPos;
	}
}

// Função para carregar textura
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

// Função para verificar colisão
bool CheckCollision(Sprite& one, Sprite& two) {
	// Calcula AABBs para ambos os sprites
	one.getAABB();
	two.getAABB();

	// Verifica se há sobreposição nos eixos x e y
	bool collisionX = one.getPMax().x >= two.getPMin().x && two.getPMax().x >= one.getPMin().x;
	bool collisionY = one.getPMax().y >= two.getPMin().y && two.getPMax().y >= one.getPMin().y;

	// Se houver sobreposição em ambos os eixos, há uma colisão
	return collisionX && collisionY;
}

// Funções para carregar o mapa
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
	// Para dar a sensação de 3D
	tileSize.y = tileSize.y / 2;
}

// Função para configurar os tiles
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

// Função para calcular a posição de desenho de cada tile (para diamond view)
glm::vec2 calculoPosicaoDesenho(int column, int row, float tileWidth, float tileHeight) {
	float x = column * tileWidth / 2.0f + row * tileWidth / 2.0f;
	float y = column * tileHeight / 2.0f - row * tileHeight / 2.0f;
	return glm::vec2(x, y);
}

// Função para calcular a posição de cada tile no mundo
glm::vec2 calcularPosicaoTile(glm::vec2 posInicial, glm::vec2 posDesenho, glm::vec2 tileSize, float rotacao) {
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(posInicial.x + posDesenho.x, posInicial.y + posDesenho.y, 0.0));
	model = glm::scale(model, glm::vec3(tileSize.x, tileSize.y, 1.0));
	model = glm::rotate(model, glm::radians(rotacao), glm::vec3(0.0, 0.0, 1.0));

	glm::vec4 originalPosition = glm::vec4(0.0, 0.0, 0.0, 1.0); // posição do centro do tile
	glm::vec4 transformedPosition = model * originalPosition;

	return glm::vec2(transformedPosition.x, transformedPosition.y);
}

// Função para desenhar o mapa
void drawMap(Shader& shader) {
	shader.Use();

	glBindTexture(GL_TEXTURE_2D, tilesetTexID);
	glBindVertexArray(VAOTile);

	for (int i = 0; i < tilemapSize.y; i++) {
		for (int j = 0; j < tilemapSize.x; j++) {
			glm::vec2 posDesenho = calculoPosicaoDesenho(j, i, tileSize.x, tileSize.y);

			glm::vec2 posTile = calcularPosicaoTile(posIni, posDesenho, tileSize, 45.0f);

			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(posTile.x, posTile.y, 0.0));
			model = glm::scale(model, glm::vec3(tileSize.x, tileSize.y, 1.0));
			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

			shader.setMat4("model", glm::value_ptr(model));

			int indiceTile = tilemap[i][j];
			shader.setVec2("offsetTex", indiceTile * offsetTex.x, offsetTex.y);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

