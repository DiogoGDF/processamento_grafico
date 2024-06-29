#include "Sprite.h"

Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &VAO);
}

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

	vel = 5.0;

	if (largura == 1) {
		offsetTex.s = 1.0/ (float) nFrames;
		offsetTex.t = 1.0/ (float) nAnimations; 
	}
	else {
		offsetTex.s = largura;
		offsetTex.t = 1.0/ (float) nAnimations;
	}

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

	FPS = 10.0;
	lastTime = 0.0;

}

void Sprite::moverParaDireita()
{
	pos.x += vel;
	if (escala.x < 0.0)
		escala.x = -escala.x;

}

void Sprite::moverParaEsquerda()
{
	pos.x -= vel;
	if (escala.x > 0.0)
		escala.x = -escala.x;
}

void Sprite::atualizar()
{
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

    glm::mat4 model = glm::mat4(1); //matriz identidade
    model = glm::translate(model, pos);
	model = glm::rotate(model,glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, escala);
    shader->setMat4("model", glm::value_ptr(model));
}

void Sprite::desenhar()
{
    atualizar();


	glBindTexture(GL_TEXTURE_2D, texID); //Conectando com a textura

    glBindVertexArray(VAO); //Conectando ao buffer de geometria

	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0); //desconectando o buffer de geometria
	glBindTexture(GL_TEXTURE_2D, 0);
}
