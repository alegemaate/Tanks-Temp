#ifndef PARTICLE_H
#define PARTICLE_H

#define CIRCLE 0
#define SQUARE 1
#define PIXEL 2
#define RANDOM 3

#include <allegro.h>
#include "tools.h"

class particle{
  public:
    particle(int newX, int newY, int newColor, int newXMin, int newXMax, int newYMin, int newYMax, int newSize, int newType, int newLife);
    ~particle();

    void logic();
    void draw(BITMAP* tempBitmap);

    bool getDead();
  private:
    int xMax;
    int xMin;
    int yMin;
    int yMax;

    int particleColor;
    int particleRed;
    int particleGreen;
    int particleBlue;
    int particleType;
    int particleSize;
    int particleLife;

    bool dead;

    float x;
    float y;
};

#endif

