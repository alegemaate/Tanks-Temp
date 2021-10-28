#ifndef SRC_GAME_BULLET_H_
#define SRC_GAME_BULLET_H_

#include <vector>

#include "../util/tools.h"
#include "./particle.h"
#include "./world.h"

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define TANK 4

class Bullet {
 public:
  Bullet(world* world,
         float x,
         float y,
         float angle,
         float speed,
         int health,
         SAMPLE* sound);
  ~Bullet();

  bool getErase();
  void update();
  void draw(BITMAP* buffer);

  float getX();
  float getY();

  float getYVelocity();
  float getXVelocity();

  void bounce(int direction);
  void destroy();

  void reverseDirection(std::string direction);

 protected:
 private:
  int health;
  int incidenceDirection;

  float x;
  float y;
  float vector_x;
  float vector_y;

  world* worldPointer;

  bool owner;
  bool pendingErase;

  SAMPLE* shotSound;
};

#endif  // SRC_GAME_BULLET_H_
