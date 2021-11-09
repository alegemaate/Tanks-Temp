#ifndef SRC_GAME_BULLET_H_
#define SRC_GAME_BULLET_H_

#include <vector>

#include "../util/tools.h"
#include "./particle.h"
#include "./world.h"

enum class BounceDirection {
  NONE,
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
};

class Bullet {
 public:
  Bullet(World* world, float x, float y, float angle, float speed, int health);

  bool getErase() const;
  void update();
  void draw(BITMAP* buffer) const;

  float getX() const;
  float getY() const;

  float getYVelocity() const;
  float getXVelocity() const;

  void bounce(BounceDirection direction);
  void destroy();

  void reverseDirection(const std::string& direction);

 private:
  World* worldPointer;

  float x;
  float y;
  float vector_x;
  float vector_y;

  int health;
  BounceDirection incidenceDirection = BounceDirection::NONE;

  bool pendingErase = false;

  SAMPLE* shotSound;
};

#endif  // SRC_GAME_BULLET_H_
