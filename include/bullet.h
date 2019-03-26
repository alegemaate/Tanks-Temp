#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

enum bullet_directions {
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
  TANK
};

class Bullet : public Entity {
  public:
    Bullet(World *wrld, float x, float y, float angle, float speed, int ownerID, int health, SAMPLE* sound);
    ~Bullet();

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;
    virtual void Collide(Entity *other) override;

    int GetOwner();

  private:
    void Bounce(int newDirection);
    void Destroy();

    int health;
    int lastBounceDirection;
    int owner;

    SAMPLE* shotSound;
};

#endif // BULLET_H
