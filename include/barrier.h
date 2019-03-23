#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "Entity.h"
#include "tools.h"

class world;
class bullet;

class Barrier : public Entity {
  public:
    Barrier(world *newWorld, float x, float y, BITMAP* image, int health);
    ~Barrier();

    void Update();
    void Draw(BITMAP* tempImage);

    void SetHealth( int newHealth){ health = newHealth; }

    void SetIndestructable( bool newIndestructable){ indestructable = newIndestructable; }

    bool GetDead();

    bool visible;
  private:
    int health;

    bool indestructable;
    bool exploded;

    void Explode( int newX, int newY, int newVelocity, int newAmount, int newLife);

    BITMAP *image;
    SAMPLE *sample_explode;
};

#endif // BARRIER_H
