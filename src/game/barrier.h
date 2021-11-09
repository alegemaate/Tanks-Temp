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

  void update(const std::vector<Bullet*>* bullets);
  void draw(BITMAP* image);

  int getWidth() const;
  int getHeight() const;

  void makeIndestructable(bool indestructable) {
    this->indestructable = indestructable;
  }

  bool getDead() const;

  Coordinate getPosition() const;

 private:
  Coordinate position;

  World* worldPointer;

  int health;

  bool indestructable = false;
  bool exploded = false;
  bool visible = true;

  float width;
  float height;

  SAMPLE* sample_explode;

  BITMAP* image;

  void explode(float x, float y, int velocity, int amount, int life);
};

#endif  // SRC_GAME_BARRIER_H_
