#ifndef SRC_GAME_PARTICLE_H_
#define SRC_GAME_PARTICLE_H_

#include <allegro.h>
#include "../util/tools.h"

enum class ParticleType {
  CIRCLE,
  SQUARE,
  PIXEL,
  RANDOM,
};

enum class ParticleBehaviour {
  EXPLODE,
};

class Particle {
 public:
  Particle(float x,
           float y,
           int color,
           float xVelocityMin,
           float xMax,
           float yMin,
           float yMax,
           int size,
           ParticleType type,
           int life,
           ParticleBehaviour behavior);

  void logic();

  void draw(BITMAP* buffer) const;

  bool getDead() const;

 private:
  void drawRandom(BITMAP* buffer) const;

  float x;
  float y;

  int color;

  int size;
  ParticleType type;
  int life;
  ParticleBehaviour behaviour;

  float xVelocity;
  float yVelocity;

  bool dead = false;
};

#endif  // SRC_GAME_PARTICLE_H_
