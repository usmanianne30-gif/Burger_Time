#ifndef BURGER_H
#define BURGER_H

#include "Entity.h"
#include "Level.h"

enum BurgerType {
    BURGER_TOP,
    BURGER_MEAT,
    BURGER_BOTTOM
};

class Burger : public Entity {
public:
    Burger(int x, int y, Level* level, BurgerType type);
    void update() override;
    void render(class Renderer* renderer, int tileSize) override;
    void drop();
    bool isFalling() const;
    
    void markTouched();
    bool isTouched() const;

private:
    Level* level;
    BurgerType type;
    bool falling;
    bool touched;
    int fallDistance;
};

#endif
