#include "Game.h"
#include "Renderer.h"
#include <windows.h>
#include <iostream>

Game::Game() : isRunning(true), gameOver(false), gameWon(false) {
    // Ensure we are in the executable's directory
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string dir = std::string(buffer).substr(0, pos);
    SetCurrentDirectory(dir.c_str());

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    renderer = new Renderer(screenWidth, screenHeight, "Burger Time Neon", true);
    
    // Load textures
    renderer->loadTexture("player", "player_neon.png");
    renderer->loadTexture("enemy", "enemy_neon.png");
    renderer->loadTexture("burger_bun", "burger_bun_neon.png");
    renderer->loadTexture("burger_meat", "burger_meat_neon.png");
    renderer->loadTexture("wall", "wall_neon.png");
    renderer->loadTexture("ladder", "ladder_neon.png");
    renderer->loadTexture("floor", "floor_neon.png");

    level = new Level();
    level->loadLevel(1);
    player = new Player(3, 2, level); // Start position on the first floor segment
    enemies.push_back(new Enemy(10, 8, level, player));
    
    // Create burgers on ladders (column 3 has ladders at rows 3, 4, 7)
    burgers.push_back(new Burger(3, 3, level, BURGER_TOP));     // On ladder
    burgers.push_back(new Burger(3, 4, level, BURGER_MEAT));    // On ladder  
    burgers.push_back(new Burger(3, 7, level, BURGER_BOTTOM));  // On ladder
}

Game::~Game() {
    delete renderer;
    delete level;
    delete player;
    for (auto enemy : enemies) delete enemy;
    for (auto burger : burgers) delete burger;
}

void Game::run() {
    while (isRunning) {
        if (!renderer->handleMessages()) {
            isRunning = false;
            break;
        }
        processInput();
        update();
        render();
        Sleep(16); // ~60 FPS
    }
}

void Game::processInput() {
    // Normal gameplay input
    if (GetAsyncKeyState('W') & 0x8000) player->move(0, -1);
    if (GetAsyncKeyState('S') & 0x8000) player->move(0, 1);
    if (GetAsyncKeyState('A') & 0x8000) player->move(-1, 0);
    if (GetAsyncKeyState('D') & 0x8000) player->move(1, 0);
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) isRunning = false;
}

void Game::update() {
    if (gameOver || gameWon) return;
    
    player->update();
    
    // Check for enemy collision with player
    for (auto enemy : enemies) {
        enemy->update();
        if (enemy->getX() == player->getX() && enemy->getY() == player->getY()) {
            // Restart game from beginning
            delete player;
            for (auto e : enemies) delete e;
            for (auto b : burgers) delete b;
            enemies.clear();
            burgers.clear();
            
            player = new Player(3, 2, level);
            enemies.push_back(new Enemy(10, 8, level, player));
            burgers.push_back(new Burger(3, 3, level, BURGER_TOP));
            burgers.push_back(new Burger(3, 4, level, BURGER_MEAT));
            burgers.push_back(new Burger(3, 7, level, BURGER_BOTTOM));
            return;
        }
    }
    
    for (auto burger : burgers) {
        burger->update();
        // Check if player stepped on burger
        if (player->getX() == burger->getX() && player->getY() == burger->getY()) {
            burger->drop();
            burger->markTouched();
        }
    }
    
    // Check win condition
    bool allTouched = true;
    for (auto burger : burgers) {
        if (!burger->isTouched()) {
            allTouched = false;
            break;
        }
    }
    
    if (allTouched) {
        gameWon = true;
    }
}

void Game::render() {
    renderer->beginFrame();
    
    int screenHeight = renderer->getHeight();
    int screenWidth = renderer->getWidth();
    int tileSize = screenHeight / 11; // 11 tiles high map
    
    level->render(renderer, tileSize);
    for (auto burger : burgers) burger->render(renderer, tileSize);
    for (auto enemy : enemies) enemy->render(renderer, tileSize);
    player->render(renderer, tileSize);
    
    int collectedCount = 0;
    for (auto burger : burgers) {
        if (burger->isTouched()) collectedCount++;
    }
    
    std::string debugText = "Collected: " + std::to_string(collectedCount) + "/3";
    renderer->drawText(debugText, 10, 10, "Arial", 20, 1.0f, 1.0f, 1.0f);

    if (gameWon) {
        renderer->drawText("YOU WIN!", screenWidth / 2 - 150, screenHeight / 2 - 50, "Comic Sans MS", 50, 1.0f, 1.0f, 0.0f);
    }
    
    renderer->endFrame();
}
