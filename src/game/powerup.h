#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <allegro.h>
#include <string>

enum class PowerupType { Health, Speed, FireSpeed, FireDelay };
class Powerup {
 public:
  Powerup();
  Powerup(int x, int y, PowerupType type);

  int getX() const { return x; }
  int getY() const { return y; }
  int getWidth() const { return 40; }
  int getHeight() const { return 40; }
  PowerupType getType() const { return type; }

  bool getDead() const { return dead; }

  void pickup() { dead = true; }

  void draw(BITMAP* buffer) const;

 private:
  int x;
  int y;

  PowerupType type;

  bool dead;

  std::string imageKey;
};

#endif  // SRC_GAME_POWERUP_H_
