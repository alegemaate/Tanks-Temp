#ifndef SRC_GAME_WORLD_H_
#define SRC_GAME_WORLD_H_

#include <vector>

#include "./particle.h"

class world {
 public:
  world() {}
  virtual ~world() {}

  void update();
  void draw(BITMAP* buffer);

  void addParticle(Particle* particle);

 private:
  std::vector<Particle*> particles;
};

#endif  // SRC_GAME_WORLD_H_
