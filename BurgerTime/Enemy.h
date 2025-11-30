#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Level.h"
#include "Player.h"

class Enemy : public Entity {
public:
    Enemy(int x, int y, Level* level, Player* player);
    void update() override;
    void render(class Renderer* renderer, int tileSize) override;

private:
    Level* level;
    Player* player;
    int moveTimer;
};

#endif
