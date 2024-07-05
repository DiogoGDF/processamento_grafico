//#include "Tile.h"
//#include <fstream>
//#include <iostream>
//#include <stb_image.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>
//
//Tile::Tile() : tilesetTexID(0), VAOTile(0), nTiles(0) {}
//
//Tile::~Tile() {
//    glDeleteVertexArrays(1, &VAOTile);
//}
//
//glm::vec2 calculateDrawingPosition(int column, int row, float tileWidth, float tileHeight) {
//    float x = column * tileWidth / 2.0f + row * tileWidth / 2.0f;
//    float y = column * tileHeight / 2.0f - row * tileHeight / 2.0f;
//    return glm::vec2(x, y);
//}
//
//void loadMap(string fileName)
//{
//    ifstream arqEntrada;
//    arqEntrada.open(fileName);
//    if (arqEntrada)
//    {
//        string textureName;
//        int width, height;
//        arqEntrada >> textureName >> nTiles >> tileSize.y >> tileSize.x;
//        tilesetTexID = loadTexture(textureName, width, height);
//        cout << textureName << " " << nTiles << " " << tileSize.y << " " << tileSize.x << endl;
//        arqEntrada >> tilemapSize.y >> tilemapSize.x;
//        cout << tilemapSize.y << " " << tilemapSize.x << endl;
//        for (int i = 0; i < tilemapSize.y; i++)
//        {
//            for (int j = 0; j < tilemapSize.x; j++)
//            {
//                arqEntrada >> tilemap[i][j];
//                cout << tilemap[i][j] << " ";
//            }
//            cout << endl;
//        }
//
//    }
//    else
//    {
//        cout << "Houve um problema na leitura de " << fileName << endl;
//    }
//}
//
//GLuint setupTile()
//{
//    GLuint VAO;
//
//    offsetTex.s = 1.0 / (float)nTiles;
//    offsetTex.t = 1.0;
//    glm::vec3 cor;
//    cor.r = 1.0;
//    cor.g = 0.0;
//    cor.b = 1.0;
//    GLfloat vertices[] = {
//        //x   y    z    r      g      b      s    t
//        -0.5, 0.5, 0.0, cor.r, cor.g, cor.b, 0.0, offsetTex.t, //v0
//        -0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
//         0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t, //v2
//        -0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
//         0.5,-0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, 0.0, //v3
//         0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t  //v2
//    };
//
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);
//    return VAO;
//}
//
//void drawMap(Shader& shader) {
//    shader.Use();
//
//    glBindTexture(GL_TEXTURE_2D, tilesetTexID);
//    glBindVertexArray(VAOTile);
//
//    for (int i = 0; i < tilemapSize.y; i++) {
//        for (int j = 0; j < tilemapSize.x; j++) {
//            glm::vec2 posDesenho = calculateDrawingPosition(j, i, tileSize.x, tileSize.y);
//
//            glm::mat4 model = glm::mat4(1);
//            model = glm::translate(model, glm::vec3(posIni.x + posDesenho.x, posIni.y + posDesenho.y, 0.0));
//            model = glm::scale(model, glm::vec3(tileSize.x, tileSize.y, 1.0));
//            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
//
//            shader.setMat4("model", glm::value_ptr(model));
//
//            int indiceTile = tilemap[i][j];
//            shader.setVec2("offsetTex", indiceTile * offsetTex.x, offsetTex.y);
//
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//        }
//    }
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindVertexArray(0);
//}
