#pragma once

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Sprite
{
    public:
    Sprite() {} 
    ~Sprite(); 

    void inicializar(GLuint texID, int nAnimations = 1, int nFrames = 1, glm::vec3 pos=glm::vec3(0), glm::vec3 escala = glm::vec3(1), float angulo = 0.0, glm::vec3 cor = glm::vec3(1.0, 1.0, 0.0), float largura = 1.0);
    void atualizar();
    void desenhar();
    void finalizar();
    void moverParaDireita();
    void moverParaEsquerda();


    inline void setShader(Shader *shader) { this->shader = shader; }
    inline void setAngulo(float angulo) { this->angulo = angulo; }
    protected:

    GLuint VAO;
    GLuint texID;

    glm::vec3 pos, escala;
    float angulo, largura;

    int nAnimations, nFrames, iAnimation, iFrame;
    glm::vec2 offsetTex;

    float vel;

    Shader *shader;

    float lastTime;
    float FPS;

public:
    void setPos(const glm::vec3& newPos) {
        pos = newPos;
    }
    glm::vec3 getPos() const {
        return pos;
    }
};