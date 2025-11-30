#include "Enemy.h"
#include <iostream>
#include <windows.h>
#include <cmath>

Enemy::Enemy(int x, int y, Level* level, Player* player) : Entity(x, y), level(level), player(player), moveTimer(0) {}

void Enemy::update() {
    moveTimer++;
    if (moveTimer < 10) return; // Slow down enemy movement
    moveTimer = 0;

    int dx = 0;
    int dy = 0;

    // Simple AI: Move towards player
    if (player->getX() > x) dx = 1;
    else if (player->getX() < x) dx = -1;
    
    if (player->getY() > y) dy = 1;
    else if (player->getY() < y) dy = -1;

    // Try moving in X first
    int newX = x + dx;
    int newY = y;
    char tile = level->getTile(newX, newY);
    
    if (tile != TILE_WALL && dx != 0) {
        x = newX;
    } else {
        // Try moving in Y
        newX = x;
        newY = y + dy;
        tile = level->getTile(newX, newY);
        if (tile != TILE_WALL && dy != 0) {
            // Ladder logic for enemies (simplified)
            bool onLadder = (level->getTile(x, y) == TILE_LADDER);
            bool targetIsLadder = (tile == TILE_LADDER);
            if (onLadder || targetIsLadder) {
                y = newY;
            }
        }
    }
}

#include "Renderer.h"

void Enemy::render(Renderer* renderer, int tileSize) {
    int screenWidth = renderer->getWidth();
    int mapWidthPixel = 20 * tileSize;
    int offsetX = (screenWidth - mapWidthPixel) / 2;
    
    renderer->drawSprite("enemy", offsetX + x * tileSize, y * tileSize, tileSize, tileSize);
}
