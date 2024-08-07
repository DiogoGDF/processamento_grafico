//#pragma once

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


class Sprite
{
public:
    //Sprite() {} //M�todo construtor padr�o
    Sprite() : ativo(true) {}
    ~Sprite(); //M�todo destrutor

    void inicializar(GLuint texID, int nAnimations = 1, int nFrames = 1, glm::vec3 pos = glm::vec3(0), glm::vec3 escala = glm::vec3(1), float angulo = 0.0, glm::vec3 cor = glm::vec3(1.0, 0.0, 1.0));
    void atualizar();
    void desenhar();
    void finalizar();

    void moveLeft();
    void moveRight();

    void moveItem();

    inline void setShader(Shader* shader) { this->shader = shader; }
    inline void setShaderDebug(Shader* shader) { this->shaderDebug = shader; }
    inline void setAngulo(float angulo) { this->angulo = angulo; }

    inline void setPosicao(glm::vec3 novaPos) { this->pos = novaPos; }
    glm::vec3 getPosicao() { return this->pos; }

    inline glm::vec3 getPMin() { return this->pmin; }
    inline glm::vec3 getPMax() { return this->pmax; }

    void setColisao() { colidiu = true; }

    void getAABB();
    bool ativo; 

protected:

    GLuint VAO; //Identificador do buffer de geometria VAO
    GLuint texID; //Identificador da textura

    //Transforma��es na geometria
    glm::vec3 pos, escala;
    float angulo;
    float vel;

    //Anima��o por sprite
    int nAnimations, nFrames, iAnimation, iFrame;
    glm::vec2 offsetTex; //ds e dt 

    float lastTime; 
    float FPS;

    Shader* shader;
    Shader* shaderDebug;

    glm::vec3 pmin, pmax;
    bool colidiu;

};