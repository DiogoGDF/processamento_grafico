#pragma once

#include "Shader.h"

class Sprite
{
public:
	Sprite() {}
	~Sprite();
	void inicializar(GLuint texID,glm::vec3 pos = glm::vec3(0.0,0.0,0.0), glm::vec3 escala = glm::vec3(1.0,1.0,1.0),float angulo = 0.0, float altura = 0.0, float largura = 0.0);
	void desenhar();

	inline void setShader(Shader* shader) { this->shader = shader; }
protected:
	void atualizar();
	GLuint VAO; //identificador do buffer de geometria, indicando os atributos dos vértices
	GLuint texID; //identificador do buffer de textura

	//Variáveis com as infos para aplicar as transformações no objeto
	glm::vec3 pos, escala;
	float angulo, altura, largura;

	//Uma referência ao programa de shader que a sprite irá usar para seu desenho
	Shader *shader;

	//Atributos para controle das ações
	float lastTime; 
	float FPS;

};

