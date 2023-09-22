#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <asw/asw.h>
#include <string>

enum class PowerUpType { HEALTH, SPEED, FIRE_SPEED, FIRE_DELAY };

class PowerUp {
 public:
  PowerUp(float x, float y, PowerUpType type);

  float getX() const { return x; }
  float getY() const { return y; }
  static float getWidth() { return 40.0f; }
  static float getHeight() { return 40.0f; }
  PowerUpType getType() const { return type; }

  bool getDead() const { return dead; }

  void pickup() { dead = true; }

  void draw() const;

 private:
  float x;
  float y;

  PowerUpType type;

  bool dead = false;

  std::string imageKey;
};

#endif  // SRC_GAME_POWERUP_H_
