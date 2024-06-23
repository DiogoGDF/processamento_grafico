// aluno: Diogo Garbinato de Fagundes	Matricula: 1189650
#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;

//Classe para manipula��o dos shaders
#include "Shader.h"

// Prot�tipo da fun��o de callback da GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Dimens�es da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Retangulos e matriz de cores
const int COLUMNS = 5, LINES = 11;
glm::vec3 colorMatrix[COLUMNS][LINES];

// Vari�veis globais
int tentativas = 0;
int retangulosRemovidos = 0;

// Prot�tipos das fun��es
int setup();
void pickColor(GLdouble xpos, GLdouble ypos);
void initRandomColors();
void removeColor(int r, int g, int b);

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Inicializa o gerador de n�meros aleat�rios (para sortear as cores)
	std::srand(std::time(0));

	// Vers�o do OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das cores", nullptr, nullptr);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o shader
	Shader shader("../shaders/retangulo.vs", "../shaders/retangulo.fs");
	Shader shaderPalete("../shaders/palete.vs", "../shaders/palete.fs");

	// Gerando um buffer simples
	GLuint VAO = setup();

	// inicializa a matriz model
	glm::mat4 model = glm::mat4(1);

	initRandomColors(); // Inicializa a matriz de cores

	// Loop principal
	while (!glfwWindowShouldClose(window))
	{

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria

		// shader dos retangulos
		shader.Use();

		GLfloat xInitialPos = -0.77f,
			yInitialPos = 0.90f;

		// Desenha os ret�ngulos
		for (int c = 0; c < COLUMNS; c++)
			for (int l = 0; l < LINES; l++) {
				// Define a cor do ret�ngulo
				shader.setVec3("cor", colorMatrix[c][l].r, colorMatrix[c][l].g, colorMatrix[c][l].b);

				// re-inicializa a matriz model
				model = glm::mat4(1);

				// Posiciona o ret�ngulo
				model = glm::translate(model, glm::vec3(xInitialPos + c * 0.385, yInitialPos - l * 0.125, 0));
				model = glm::scale(model, glm::vec3(0.38, 0.38, 1));

				// Envia a matriz model para o shader
				shader.setMat4("model", glm::value_ptr(model));

				// Desenha o ret�ngulo
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}

		// shader da paleta
		shaderPalete.Use();

		// Reinicializa a matriz model
		model = glm::mat4(1);

		// Posiciona a paleta de cores
		model = glm::translate(model, glm::vec3(0.70, -0.70, 0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 1));

		// envia a matriz para o shader
		shaderPalete.setMat4("model", glm::value_ptr(model));

		// Desenha a paleta de cores
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Fecha a aplica��o se a tecla ESC for pressionada
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Fun��o de callback de clique do bot�o
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLdouble xpos, ypos;
	int w, h;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Obt�m a posi��o do mouse
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwGetWindowSize(window, &w, &h);

		// Pega a cor do pixel clicado
		pickColor(xpos, h - ypos);
	}
}

int setup()
{
	// Definindo um gradiente de cores para a paleta
	GLfloat vertices[] = {
		-0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice ret�ngulo
		-0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // V�rtice ret�ngulo
		0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // V�rtice ret�ngulo
		0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,    // V�rtice ret�ngulo 
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // V�rtice cores  (R)
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // V�rtice cores  (G)
		0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // V�rtice cores  (B)
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f   // V�rtice cores  (RGB)	
	};

	GLuint VBO, VAO;

	// Gera e liga o buffer de v�rtices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Atributo 0 - posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desvincula o VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO
	glBindVertexArray(0);

	return VAO;
}

// Fun��o para pegar a cor do pixel clicado
void pickColor(GLdouble xpos, GLdouble ypos) {
	unsigned char pixel[4];
	glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
	if (pixel[0] || pixel[1] || pixel[2]) // se a cor for diferente de preto
	{
		// Incrementa o n�mero de tentativas
		tentativas += 1;
		if (retangulosRemovidos < COLUMNS * LINES)
			cout << "Tentativas: " << tentativas << endl;

		// Chama a fun��o para remover a cor
		removeColor((int)pixel[0], (int)pixel[1], (int)pixel[2]);
	}
}

// Fun��o para inicializar as cores dos ret�ngulos
void initRandomColors()
{
	// Fun��o para sortear e armazenar as cores em uma colorMatrix[COLUMNS][LINES] de glm::vec3
	for (int c = 0; c < COLUMNS; c++)
		for (int l = 0; l < LINES; l++)
		{
			// Sorteia valores de cor aleat�rios para RGB
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

			// Armazena as cores na matriz
			colorMatrix[c][l] = glm::vec3(r, g, b);
		}
}

// Fun��o para remover a cor clicada
void removeColor(int r, int g, int b) {
	// Toler�ncia, alterar para mudar a dificuldade (quanto menor, mais dif�cil)
	float tolerance = 0.45f;

	// Converte os valores de RGB para a faixa de 0 a 1
	float red = static_cast<float>(r) / 255.0f;
	float green = static_cast<float>(g) / 255.0f;
	float blue = static_cast<float>(b) / 255.0f;

	// Calcula os limites superior e inferior para a toler�ncia
	float lowerRed = max(0.0f, red - tolerance);
	float upperRed = min(1.0f, red + tolerance);
	float lowerGreen = max(0.0f, green - tolerance);
	float upperGreen = min(1.0f, green + tolerance);
	float lowerBlue = max(0.0f, blue - tolerance);
	float upperBlue = min(1.0f, blue + tolerance);

	// Percorre todos os pixels da matriz de cores
	for (int c = 0; c < COLUMNS; c++)
		for (int l = 0; l < LINES; l++)
		{
			// Verifica se a cor do pixel est� dentro da toler�ncia
			if (colorMatrix[c][l].r >= lowerRed && colorMatrix[c][l].r <= upperRed &&
				colorMatrix[c][l].g >= lowerGreen && colorMatrix[c][l].g <= upperGreen &&
				colorMatrix[c][l].b >= lowerBlue && colorMatrix[c][l].b <= upperBlue)
			{
				// Troca a cor do pixel para preto (com negativos para n�o ter problemas com a toler�ncia)
				colorMatrix[c][l] = glm::vec3(-1.0f, -1.0f, -1.0f);
				retangulosRemovidos += 1;
				cout << "Retangulos removidos: " << retangulosRemovidos << endl;

				// Fim do jogo
				if (retangulosRemovidos == COLUMNS * LINES)
					cout << "\nParabens! Voce terminou em " << tentativas << " tentativas!" << endl;
			}
		}
}