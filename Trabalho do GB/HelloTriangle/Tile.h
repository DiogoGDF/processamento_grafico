//#pragma once
//
//#include <string>
//#include <glm/glm.hpp>
//#include <glad/glad.h>
//
//#include "Shader.h"
//#include "Source.cpp"
//
//class Tile {
//public:
//    Tile();
//    ~Tile();
//
//    void loadMap(string fileName);
//    glm::vec2 calculateDrawingPosition(int column, int row, float tileWidth, float tileHeight);
//    GLuint setupTile();
//    void drawMap(Shader& shader);
//
//    GLuint getTilesetTexID() const { return tilesetTexID; }
//    glm::vec2 getTileSize() const { return tileSize; }
//
//private:
//    GLuint tilesetTexID;
//    GLuint VAOTile;
//    int nTiles;
//    glm::vec2 tileSize;
//    glm::vec2 tilemapSize;
//    int tilemap[15][15];
//    glm::vec2 offsetTex;
//
//};
