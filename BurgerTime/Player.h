#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Level.h"

class Player : public Entity {
public:
    Player(int x, int y, Level* level);
    void update() override;
    void move(int dx, int dy);
    void render(class Renderer* renderer, int tileSize) override;

private:
    Level* level;
    int moveTimer;
};

#endif
