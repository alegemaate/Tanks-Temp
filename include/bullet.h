#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "tools.h"

#include <vector>

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define TANK 4


class Bullet : public Entity {
  public:
    Bullet(world *wrld, float x, float y, float angle, float speed, int ownerID, int health, SAMPLE* sound);
    ~Bullet();

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;
    virtual void Collide(Entity *other) override;

    void Bounce(int newDirection);
    void Destroy();
  protected:
  private:
    int health;
    int lastBounceDirection;
    bool owner;

    SAMPLE* shotSound;
};

#endif // BULLET_H
