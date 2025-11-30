#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    Entity(int x, int y);
    virtual ~Entity() {}

    virtual void update() = 0; // Pure virtual function
    virtual void render(class Renderer* renderer, int tileSize) = 0;
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

protected:
    int x, y;
};

#endif
