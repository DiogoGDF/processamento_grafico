#include "Sprite.h"

class Tile : public Sprite
{
public:
    Tile() { } //Método construtor padrão

    void inicializar(GLuint texID, int nRows = 1, int n = 1, glm::vec3 pos = glm::vec3(0), glm::vec3 escala = glm::vec3(1), float angulo = 0.0, glm::vec3 cor = glm::vec3(1.0, 0.0, 1.0));
    void atualizar();
    void desenhar();
protected:

    int nRows, nCols; //linhas e colunas do tileset

};
