#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <allegro.h>
#include <string>

enum class PowerupType { Health, Speed, FireSpeed, FireDelay };
class Powerup {
 public:
  Powerup();
  Powerup(int x, int y, PowerupType type);

  int getX() { return x; }
  int getY() { return y; }
  int getWidth() { return 40; }
  int getHeight() { return 40; }
  PowerupType getType() { return type; }

  bool getDead() { return dead; }

  void pickup() { dead = true; }

  void draw(BITMAP* buffer);

 private:
  int x;
  int y;

  PowerupType type;

  bool dead;

  std::string imageKey;
};

#endif  // SRC_GAME_POWERUP_H_
