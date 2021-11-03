#ifndef SRC_GAME_PLAYER_TANK_H_
#define SRC_GAME_PLAYER_TANK_H_

#include "tank.h"

class PlayerTank : public Tank {
 public:
  PlayerTank(World* world,
             int x,
             int y,
             int hurtTime,
             int health,
             int fireSpeed,
             int fireDelay,
             float speed);
  virtual void update();

 protected:
};

#endif  // SRC_GAME_PLAYER_TANK_H_
