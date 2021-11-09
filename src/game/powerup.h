#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <allegro.h>
#include <string>

enum class PowerupType { Health, Speed, FireSpeed, FireDelay };
class Powerup {
 public:
  Powerup();
  Powerup(float x, float y, PowerupType type);

  float getX() const { return x; }
  float getY() const { return y; }
  float getWidth() const { return 40.0f; }
  float getHeight() const { return 40.0f; }
  PowerupType getType() const { return type; }

  bool getDead() const { return dead; }

  void pickup() { dead = true; }

  void draw(BITMAP* buffer) const;

 private:
  float x;
  float y;

  PowerupType type;

  bool dead;

  std::string imageKey;
};

#endif  // SRC_GAME_POWERUP_H_
