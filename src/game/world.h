#ifndef SRC_GAME_WORLD_H_
#define SRC_GAME_WORLD_H_

#include <vector>

#include "particle.h"

class World {
 public:
  void update();
  void draw(BITMAP* buffer) const;

  void addParticle(Particle* particle);

 private:
  std::vector<Particle*> particles;
};

#endif  // SRC_GAME_WORLD_H_
