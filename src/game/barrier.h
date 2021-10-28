#ifndef SRC_GAME_BARRIER_H_
#define SRC_GAME_BARRIER_H_

#include <vector>

#include "../util/tools.h"
#include "./bullet.h"

class Barrier {
 public:
  Barrier(world* world, int x, int y, BITMAP* image, int health);
  ~Barrier();

  void update(std::vector<Bullet>* bullets);
  void draw(BITMAP* image);

  int getX();
  int getY();

  int getWidth();
  int getHeight();

  void setHealth(int health) { this->health = health; }

  void setIndestructable(bool indestructable) {
    this->indestructable = indestructable;
  }

  bool getDead();

 private:
  int x;
  int y;

  BITMAP* image;
  world* worldPointer;

  int health;

  bool indestructable;
  bool exploded;
  bool visible;

  int width;
  int height;

  SAMPLE* sample_explode;

  void explode(int x, int y, int velocity, int amount, int life);
};

#endif  // SRC_GAME_BARRIER_H_
