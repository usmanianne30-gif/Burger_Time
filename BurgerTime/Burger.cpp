#include "Burger.h"
#include <iostream>
#include <windows.h>

Burger::Burger(int x, int y, Level* level, BurgerType type) : Entity(x, y), level(level), type(type), falling(false), touched(false), fallDistance(0) {}

void Burger::update() {
    if (falling) {
        // Fall logic
        int newY = y + 1;
        char tile = level->getTile(x, newY);
        
        bool canFall = true;
        if (tile == TILE_WALL) {
            canFall = false;
        } else if (tile == TILE_FLOOR) {
            // Check if we can fall through the floor
            // Allow falling through if there is a ladder below
            char tileBelow = level->getTile(x, newY + 1);
            if (tileBelow != TILE_LADDER) {
                canFall = false;
            }
        }
        
        if (canFall) {
             y = newY;
        } else {
            // Stop falling
            falling = false;
        }
    }
}

void Burger::drop() {
    falling = true;
}

bool Burger::isFalling() const {
    return falling;
}

void Burger::markTouched() {
    touched = true;
}

bool Burger::isTouched() const {
    return touched;
}

#include "Renderer.h"

void Burger::render(Renderer* renderer, int tileSize) {
    int screenWidth = renderer->getWidth();
    int mapWidthPixel = 20 * tileSize;
    int offsetX = (screenWidth - mapWidthPixel) / 2;
    
    std::string texture = "burger_bun";
    if (type == BURGER_MEAT) texture = "burger_meat";
    
    renderer->drawSprite(texture, offsetX + x * tileSize, y * tileSize, tileSize, tileSize);
}
