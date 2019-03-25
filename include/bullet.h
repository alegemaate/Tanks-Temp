#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "tools.h"
#include "particle.h"

#include <vector>

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define TANK 4


class bullet : public Entity {
  public:
    bullet(world *wrld, float x, float y, float angle, float speed, int ownerID, int health, SAMPLE* sound);
    ~bullet();

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;

    void Bounce( int newDirection);
    void Destroy();

    void ReverseDirection( std::string newDirection);
  protected:
  private:
    int health;
    int incidenceDirection;

    bool owner;

    SAMPLE* shotSound;
};

#endif // BULLET_H
