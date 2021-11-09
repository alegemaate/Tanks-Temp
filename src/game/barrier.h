#ifndef SRC_GAME_BARRIER_H_
#define SRC_GAME_BARRIER_H_

#include <vector>

#include "../util/coordinate.h"
#include "./bullet.h"

enum class BarrierType {
  NONE,
  BOX,
  STONE,
};

class Barrier {
 public:
  Barrier(World* world, Coordinate position, BarrierType type);
  ~Barrier();

  void update(std::vector<Bullet*>* bullets);
  void draw(BITMAP* image);

  int getWidth();
  int getHeight();

  void makeIndestructable(bool indestructable) {
    this->indestructable = indestructable;
  }

  bool getDead();

  Coordinate position;

 private:
  World* worldPointer;

  BarrierType type;

  int health;

  bool indestructable;
  bool exploded;
  bool visible;

  float width;
  float height;

  SAMPLE* sample_explode;

  BITMAP* image;

  void explode(float x, float y, int velocity, int amount, int life);
};

#endif  // SRC_GAME_BARRIER_H_
