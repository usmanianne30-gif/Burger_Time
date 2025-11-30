#include "Player.h"
#include <iostream>
#include <windows.h>

Player::Player(int x, int y, Level* level) : Entity(x, y), level(level), moveTimer(0) {}

void Player::update() {
    if (moveTimer > 0) moveTimer--;
}

void Player::move(int dx, int dy) {
    if (moveTimer > 0) return;

    int newX = x + dx;
    int newY = y + dy;

    char tile = level->getTile(newX, newY);
    char currentTile = level->getTile(x, y);

    // Strict collision detection
    // Must move onto a valid tile (Floor, Ladder, or Burger Plate)
    // Cannot move into Wall
    if (tile != TILE_WALL) {
        bool canMove = false;

        // Vertical movement: Must be on or moving to a ladder
        if (dy != 0) {
            if (currentTile == TILE_LADDER || tile == TILE_LADDER) {
                canMove = true;
            }
        }
        // Horizontal movement: Must be on a floor or ladder
        else if (dx != 0) {
            if (tile == TILE_FLOOR || tile == TILE_LADDER || tile == TILE_PLATE) {
                canMove = true;
            }
        }

        if (canMove) {
            x = newX;
            y = newY;
            moveTimer = 5; // Cooldown frames
        }
    }
}

#include "Renderer.h"

void Player::render(Renderer* renderer, int tileSize) {
    int screenWidth = renderer->getWidth();
    int mapWidthPixel = 20 * tileSize;
    int offsetX = (screenWidth - mapWidthPixel) / 2;
    
    renderer->drawSprite("player", offsetX + x * tileSize, y * tileSize, tileSize, tileSize);
}
