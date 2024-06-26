#include "Sprite.h"

Sprite::~Sprite()
{
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::inicializar(GLuint texID, glm::vec3 pos, glm::vec3 escala, float angulo, float altura, float largura)
{
	this->texID = texID;
	this->pos = pos;
	this->escala = escala;
	this->angulo = angulo;
	this->altura = altura;
	this->largura = largura;

	cout << "Altura: " << altura << " Largura: " << largura << endl;

	GLfloat vertices[] = {
		//Triangulo 0
		//x     y    z		r    g    b			s    t
		-0.5 , 0.5, 0.0,	1.0, 0.0, 0.0,		0.0, altura,  //v0
		-0.5 ,-0.5, 0.0,	1.0, 0.0, 0.0,		0.0, 0.0,  //v1
		 0.5 , 0.5, 0.0,	1.0, 0.0, 0.0,		largura, altura,  //v2

		 //Triangulo 1	
	    -0.5 ,-0.5, 0.0,	1.0, 0.0, 0.0,		0.0, 0.0,  //v1
		 0.5 ,-0.5, 0.0,	1.0, 0.0, 0.0,		largura, 0.0,  //v3
		 0.5 , 0.5, 0.0,	1.0, 0.0, 0.0,		largura, altura //v2
		
	};
	
	// tileset e tilemap
	// mapeamento de textura

	GLuint VBO;
	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//                                              vertices.data()  
	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo 2 - coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO 
	glBindVertexArray(0);

}

void Sprite::desenhar()
{
	atualizar();

	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO); //Conectando ao buffer de geometria
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind
	glBindVertexArray(0); //unbind
}


void Sprite::atualizar()
{
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, escala);
	shader->setMat4("model", glm::value_ptr(model));
}
