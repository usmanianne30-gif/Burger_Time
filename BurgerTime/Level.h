#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <iostream>

// Tile types
const char TILE_EMPTY = ' ';
const char TILE_WALL = '#';
const char TILE_LADDER = 'H';
const char TILE_FLOOR = '=';
const char TILE_PLATE = '_';

class Level {
public:
    Level();
    void loadLevel(int levelNum);
    void render(class Renderer* renderer, int tileSize);
    char getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void setTile(int x, int y, char tile);

private:
    std::vector<std::string> map;
    int width;
    int height;
};

#endif
