#ifndef PARTICLE_H
#define PARTICLE_H

#define CIRCLE 0
#define SQUARE 1
#define PIXEL 2
#define RANDOM 3

#define EXPLODE 4

#include <allegro.h>
#include "tools.h"

class Particle {
  public:
    Particle();
    Particle(int x, int y, int colour, float x_min, float x_max, float y_min, float y_max, int size, int type, int life, int behaviour);
    ~Particle();

    void Update();
    void Draw(BITMAP* buffer);

    bool IsDead();
  private:
    void SetColour(int colour);

    int colour;
    int red;
    int green;
    int blue;

    int type;
    int size;
    int life;
    int behaviour;

    bool dead;

    float x;
    float y;
    float x_velocity;
    float y_velocity;
};

#endif

