#include "Sprite.h"

Sprite::~Sprite()
{
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::inicializar(GLuint texID, int nAnimations, int nFrames, glm::vec3 pos, glm::vec3 escala, float angulo, glm::vec3 cor)
{
	this->pos = pos;
	this->escala.x = escala.x / (float)nFrames;
	this->escala.y = escala.y / (float)nAnimations;
	this->angulo = angulo;
	this->texID = texID;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;

	offsetTex.s = 1.0 / (float)nFrames;
	offsetTex.t = 1.0 / (float)nAnimations;
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
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo layout 0 - Posi��o - 3 valores dos 8 que descrevem o v�rtice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo layout 1 - Cor - 3 valores dos 8 que descrevem o v�rtice
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo layout 2 - Coordenada de textura - 2 valores dos 8 que descrevem o v�rtice
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	vel = 2.0;

	iAnimation = 0;
	iFrame = 0;

	getAABB();

	colidiu = false;
}

void Sprite::atualizar()
{
	shader->Use();
	//iFrame = (iFrame + 1) % nFrames; //incrementando ciclicamente o indice do Frame

	//Calculando o quanto teremos que deslocar nas coordenadas de textura
	float offsetTexFrameS = iFrame * offsetTex.s;
	float offsetTexFrameT = iAnimation * offsetTex.t;
	shader->setVec2("offsetTex", offsetTexFrameS, offsetTexFrameT);

	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, escala);
	shader->setMat4("model", glm::value_ptr(model));

	shaderDebug->Use();
	shaderDebug->setVec2("offsetTex", offsetTexFrameS, offsetTexFrameT);
	shaderDebug->setMat4("model", glm::value_ptr(model));

}

void Sprite::desenhar()
{
	atualizar();

	shader->Use();
	glBindTexture(GL_TEXTURE_2D, texID); //Conectando com a textura


	glBindVertexArray(VAO); //Conectando ao buffer de geometria

	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	shaderDebug->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 6);

	glBindVertexArray(0); //desconectando o buffer de geometria

}

void Sprite::moveLeft()
{
	pos.x = pos.x - vel;
}

void Sprite::moveRight()
{
	pos.x = pos.x + vel;
}

void Sprite::moveItem()
{

	if (colidiu || pos.y < 130)
	{
		//Reinicia a posi��o
		pos.y = 700;
		pos.x = 10 + rand() % 780;
		colidiu = false;
	}
	else
	{
		pos.y -= vel;
	}
}

void Sprite::getAABB()
{
	pmin.x = pos.x - escala.x / 2.0;
	pmin.y = pos.y - escala.y / 2.0;

	pmax.x = pos.x + escala.x / 2.0;
	pmax.y = pos.y + escala.y / 2.0;

}
