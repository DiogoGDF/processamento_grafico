#include <iostream>
#include <string>
#include <assert.h>

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

// soundtrack
#include <Windows.h>
#include <mmsystem.h>
#include <string>

#pragma comment(lib, "winmm.lib")


//STB_IMAGE
#include <stb_image.h>

#include "Sprite.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint loadTexture(string filePath, int &imgWidth, int &imgHeight);

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	// soundtrack
	PlaySound(TEXT("../../Soundtrack/end_of_line.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CyberCity", nullptr, nullptr);
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



	Shader shader("HelloTriangle.vs","HelloTriangle.fs");
	shader.Use();

	int imgWidth, imgHeight;
	GLuint texID = loadTexture("../../Textures/characters/PNG/Biker/Biker_idle.png", imgWidth, imgHeight);
	
	Sprite sprite, background,background2, ground, ground2;
	sprite.inicializar(texID, 1, 4, glm::vec3(400.0,60.0,0.0), glm::vec3(imgWidth*2,imgHeight*2,1.0),0.0,glm::vec3(1.0,0.0,1.0), 1.0);
	sprite.setShader(&shader);

	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Night/1.png", imgWidth, imgHeight);
	background.inicializar(texID, 1, 1, glm::vec3(450.0,300.0,0.0), glm::vec3(imgWidth*2,imgHeight*2,1.0),0.0,glm::vec3(1.0,0.0,1.0), 1.0);
	background.setShader(&shader);

	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Night/3.png", imgWidth, imgHeight);
	background2.inicializar(texID, 1, 1, glm::vec3(400.0,300.0,0.0), glm::vec3(800,600,1.0),0.0,glm::vec3(0.0,1.0,1.0), 1.0);
	background2.setShader(&shader);

	texID = loadTexture("../../Textures/backgrounds/PNG/Cybercity2/Tiles/Tile_26.png", imgWidth, imgHeight);
	ground.inicializar(texID, 1, 1, glm::vec3(400.0,0.0,0.0), glm::vec3(800,imgHeight,1.0),0.0,glm::vec3(0.0,1.0,1.0), 30.0);
	ground.setShader(&shader);

	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection",glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texBuffer", 0);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		background.desenhar();
		background2.desenhar();

		ground.desenhar();

		sprite.desenhar();

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

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
    	 std::cout << "Failed to load texture" << std::endl;
	}


	return texID;
}

