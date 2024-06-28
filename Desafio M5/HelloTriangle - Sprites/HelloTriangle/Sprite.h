#pragma once

#include "Shader.h"

class Sprite
{
public:
	Sprite() {}
	~Sprite();
	void inicializar(GLuint texID,glm::vec3 pos = glm::vec3(0), glm::vec3 escala = glm::vec3(1),float angulo = 0.0, float altura = 0.0, float largura = 0.0, float frames = 1.0, float animacoes = 1.0);
	void desenhar();

	inline void setShader(Shader* shader) { this->shader = shader; }
protected:
	void atualizar();
	GLuint VAO; //identificador do buffer de geometria, indicando os atributos dos v�rtices
	GLuint texID; //identificador do buffer de textura

	//Vari�veis com as infos para aplicar as transforma��es no objeto
	glm::vec3 pos, escala;
	float angulo, altura, largura, frames, animacoes;

	//Uma refer�ncia ao programa de shader que a sprite ir� usar para seu desenho
	Shader *shader;

	//Atributos para controle das a��es
	float lastTime; 
	float FPS;

};

