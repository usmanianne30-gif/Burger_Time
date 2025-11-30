#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Burger.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void processInput();
    void update();
    void render();

    bool isRunning;
    bool gameOver;
    bool gameWon;
    class Renderer* renderer;
    Level* level;
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<Burger*> burgers;
};

#endif
