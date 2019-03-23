#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "Entity.h"
#include "tools.h"

class Barrier : public Entity {
  public:
    Barrier(float x, float y, BITMAP* image, int health);
    ~Barrier();

    void Update();
    void Draw(BITMAP* tempImage);

    void SetHealth(int newHealth){ health = newHealth; }

    void SetIndestructable(bool newIndestructable){ indestructable = newIndestructable; }

    bool GetDead();

    bool visible;
  private:
    int health;

    bool indestructable;
    bool exploded;

    void Explode(int x, int y, int velocity, int amount, int life);

    BITMAP *image;
    SAMPLE *sample_explode;
};

#endif // BARRIER_H
