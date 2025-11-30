#include "Level.h"
#include <iostream>
#include <windows.h> // For SetConsoleCursorPosition

#include "Renderer.h"

Level::Level() : width(0), height(0) {}

void Level::loadLevel(int levelNum) {
    // Simple hardcoded level for now
    map = {
        "####################",
        "#                  #",
        "#  ======  ======  #",
        "#  H    H  H    H  #",
        "#  H    H  H    H  #",
        "#  ======  ======  #",
        "#  H            H  #",
        "#  H            H  #",
        "#  ================#",
        "#                  #",
        "####################"
    };
    height = map.size();
    width = map[0].size();
}

void Level::render(Renderer* renderer, int tileSize) {
    // Center the map horizontally
    int screenWidth = renderer->getWidth();
    int mapWidthPixel = width * tileSize;
    int offsetX = (screenWidth - mapWidthPixel) / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char tile = map[y][x];
            float drawX = offsetX + x * tileSize;
            float drawY = y * tileSize;

            if (tile == TILE_WALL) {
                renderer->drawSprite("wall", drawX, drawY, tileSize, tileSize);
            } else if (tile == TILE_LADDER) {
                // Scale down ladder by 10%
                float scale = 0.9f;
                float offset = (tileSize * (1.0f - scale)) / 2.0f;
                // Tint ladder purple if texture is missing or reused
                renderer->drawSprite("ladder", drawX + offset, drawY + offset, tileSize * scale, tileSize * scale, 1.0f, 0.0f, 1.0f);
            } else if (tile == TILE_FLOOR) {
                // Scale down floor by 10%
                float scale = 0.9f;
                float offset = (tileSize * (1.0f - scale)) / 2.0f;
                // Tint floor green if texture is missing or reused
                renderer->drawSprite("floor", drawX + offset, drawY + offset, tileSize * scale, tileSize * scale, 0.0f, 1.0f, 0.0f);
            }
        }
    }
}

char Level::getTile(int x, int y) const {
    if (y >= 0 && y < height && x >= 0 && x < width) {
        return map[y][x];
    }
    return TILE_WALL; // Treat out of bounds as walls
}

int Level::getWidth() const {
    return width;
}

int Level::getHeight() const {
    return height;
}

void Level::setTile(int x, int y, char tile) {
    if (y >= 0 && y < height && x >= 0 && x < width) {
        map[y][x] = tile;
    }
}
