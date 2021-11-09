#ifndef SRC_GAME_AI_TANK_H_
#define SRC_GAME_AI_TANK_H_

#include "tank.h"

class AiTank : public Tank {
 public:
  AiTank(World* world,
         float x,
         float y,
         int health,
         int fireSpeed,
         int fireDelay,
         float speed,
         bool enemy);
  virtual void update();

 private:
  float destination_x;
  float destination_y;

  void update_target();
  void find_enemy_target();
  void ai_drive();
};

#endif  // SRC_GAME_AI_TANK_H_
