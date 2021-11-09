#include "powerup.h"

#include "../system/ImageRegistry.h"

Powerup::Powerup() : Powerup(0, 0, PowerupType::Health) {}

Powerup::Powerup(float x, float y, PowerupType type)
    : x(x), y(y), type(type), dead(false) {
  switch (type) {
    case PowerupType::Health:
      imageKey = "powerup-health";
      break;
    case PowerupType::Speed:
      imageKey = "powerup-tank-speed";
      break;
    case PowerupType::FireSpeed:
      imageKey = "powerup-bullet-speed";
      break;
    case PowerupType::FireDelay:
      imageKey = "powerup-bullet-delay";
      break;
  }
}

void Powerup::draw(BITMAP* buffer) const {
  BITMAP* image = ImageRegistry::getImage(imageKey);
  draw_sprite(buffer, image, x, y);
}
