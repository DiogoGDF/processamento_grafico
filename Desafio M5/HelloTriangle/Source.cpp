// Aluno: Diogo Garbinato de Fagundes		Matrícula: 1189650

// Importações
#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// Glad e Glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

// Glm para operações com matrizes
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Trilha sonora
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#pragma comment(lib, "winmm.lib")


#include <stb_image.h>

#include "Sprite.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint loadTexture(string filePath, int &imgWidth, int &imgHeight);

// Dimensões da tela
const GLuint WIDTH = 800, HEIGHT = 600;

// Personagem
Sprite idle, running;
bool moving = false;

int main()
{
	// Trilha sonora
	PlaySound(TEXT("../../Soundtrack/end_of_line.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	glfwInit();

	// Versão OpenGl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Inicializando a janela
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CyberCity", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// Inicializando Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Iniciando viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Compilando shaders
	Shader shader("HelloTriangle.vs","HelloTriangle.fs");
	shader.Use();

	//  ***** Lendo texturas e inicializando sprites ****
	int imgWidth, imgHeight;
	Sprite background,background2, ground, ground2;

	// Personagem parado
	GLuint texID = loadTexture("../../Textures/characters/PNG/Biker/Biker_idle.png", imgWidth, imgHeight);
	idle.inicializar(texID, 1, 4, glm::vec3(400.0,60.0,0.0), glm::vec3(imgWidth*1.5, imgHeight * 1.5, 1.0), 0.0, glm::vec3(1.0, 0.0, 1.0), 1.0);
	idle.setShader(&shader);
	
	// Personagem correndo
	texID = loadTexture("../../Textures/characters/PNG/Biker/Biker_run.png", imgWidth, imgHeight);
	running.inicializar(texID, 1, 6, glm::vec3(400.0,60.0,0.0), glm::vec3(imgWidth*1.5,imgHeight*1.5,1.0),0.0,glm::vec3(1.0,0.0,1.0), 1.0);
	running.setShader(&shader);

	// Tela de fundo
	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Night/1.png", imgWidth, imgHeight);
	background.inicializar(texID, 1, 1, glm::vec3(450.0,300.0,0.0), glm::vec3(imgWidth*2,imgHeight*2,1.0),0.0,glm::vec3(1.0,0.0,1.0), 1.0);
	background.setShader(&shader);

	// Prédios no fundo
	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Night/3.png", imgWidth, imgHeight);
	background2.inicializar(texID, 1, 1, glm::vec3(400.0,300.0,0.0), glm::vec3(800,600,1.0),0.0,glm::vec3(0.0,1.0,1.0), 1.0);
	background2.setShader(&shader);

	// Chão
	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Tiles/Tile_26.png", imgWidth, imgHeight);
	ground.inicializar(texID, 1, 1, glm::vec3(400.0,10.0,0.0), glm::vec3(800,imgHeight,1.0),0.0,glm::vec3(0.0,1.0,1.0), 30.0);
	ground.setShader(&shader);

	// Matriz de projeção
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection",glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texBuffer", 0);
	
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Limpando cores e cor de fundo
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Desenhando os sprites
		background.desenhar();
		background2.desenhar();

		ground.desenhar();

		// Desenhando o personagem de acordo com o estado
		if (moving)
			running.desenhar();
		else
			idle.desenhar();

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}

// Função para tratar eventos de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ESC para encerrar o programa
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Movimentar o personagem para a direita
	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		moving = true;
		// Trocar o sprite para o de corrida
		running.moverParaDireita();
		if (action == GLFW_RELEASE)
		{
			moving = false;
			// Trocar o sprite para o de parado (colocando na posição do sprite de corrida)
			idle.setPos(running.getPos());
			idle.moverParaDireita();
		}
	}
	// Movimentar o personagem para a esquerda
	else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		moving = true;
		running.moverParaEsquerda();
		if (action == GLFW_RELEASE)
		{
			moving = false;
			idle.setPos(running.getPos());
			idle.moverParaEsquerda();
		}
	}
	else
	{
		moving = false;
	}


}

// Função para carregar texturas
GLuint loadTexture(string filePath, int &imgWidth, int &imgHeight)
{
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

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

