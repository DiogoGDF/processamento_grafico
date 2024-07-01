#include "Sprite.h"

// Destrutor
Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &VAO);
}

// Construtor
void Sprite::inicializar(GLuint texID, int nAnimations, int nFrames, glm::vec3 pos, glm::vec3 escala, float angulo, glm::vec3 cor, float largura)
{
    this->pos = pos;
    this->escala.x = escala.x / (float) nFrames;
	this->escala.y = escala.y / (float) nAnimations;
    this->angulo = angulo;
	this->texID = texID;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
	this->largura = largura;

	// Velocidade padrão
	vel = 5.0;

	// Parâmetro para repetir a textura quando desejado
	if (largura == 1) {
		offsetTex.s = 1.0/ (float) nFrames;
		offsetTex.t = 1.0/ (float) nAnimations; 
	}
	else {
		offsetTex.s = largura;
		offsetTex.t = 1.0/ (float) nAnimations;
	}

	// Retangulo a base de dois triângulos; offsetTex é o parâmetro para animação
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

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6* sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);

	// Frames por segundo
	FPS = 10.0;
	// iniciando o tempo
	lastTime = 0.0;
}

// Função para mover o sprite para a direita
void Sprite::moverParaDireita()
{
	// Atualiza a posição
	pos.x += vel;
	// Se a escala for negativa, inverte a escala para 
	if (escala.x < 0.0)
		escala.x = -escala.x;

}

// Função para mover o sprite para a esquerda
void Sprite::moverParaEsquerda()
{
	// Atualiza a posição
	pos.x -= vel;
	// Se a escala for positiva, inverte a escala
	if (escala.x > 0.0)
		escala.x = -escala.x;
}

// Função para atualizar o sprite
void Sprite::atualizar()
{
	// Atualizando a textura de acordo com o FPS para animação
	float now = glfwGetTime();
	float dt = now - lastTime;
	if (dt >= 1 / FPS)
	{
		iFrame = (iFrame + 1) % nFrames;
		lastTime = now;
	}
	float offsetTexFrameS = iFrame * offsetTex.s; 
	float offsetTexFrameT = iAnimation * offsetTex.t; 
	shader->setVec2("offsetTex",offsetTexFrameS,offsetTexFrameT);

	// Atualizando a posição do sprite
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, pos);
	model = glm::rotate(model,glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, escala);
    shader->setMat4("model", glm::value_ptr(model));
}

// Função para desenhar o sprite na tela
void Sprite::desenhar()
{
    atualizar();
	glBindTexture(GL_TEXTURE_2D, texID);
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0); 
	glBindTexture(GL_TEXTURE_2D, 0);
}
