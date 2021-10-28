#ifndef SRC_GAME_PARTICLE_H_
#define SRC_GAME_PARTICLE_H_

#define CIRCLE 0
#define SQUARE 1
#define PIXEL 2
#define RANDOM 3

#define EXPLODE 4

#include <allegro.h>
#include "../util/tools.h"

class Particle {
 public:
  Particle();
  Particle(int x,
           int y,
           int color,
           float xMin,
           float xMax,
           float yMin,
           float yMax,
           int size,
           int type,
           int life,
           int behavior);

  void logic();
  void draw(BITMAP* buffer);

  bool getDead();

 private:
  float x;
  float y;

  int color;

  int size;
  int type;
  int life;
  int behaviour;

  bool dead;

  float xVelocity;
  float yVelocity;
};

#endif  // SRC_GAME_PARTICLE_H_
