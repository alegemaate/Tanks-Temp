#include "powerup.h"

#include "../system/ImageRegistry.h"

Powerup::Powerup(float x, float y, PowerupType type) : x(x), y(y), type(type) {
  switch (type) {
    case PowerupType::HEALTH:
      imageKey = "powerup-health";
      break;
    case PowerupType::SPEED:
      imageKey = "powerup-tank-speed";
      break;
    case PowerupType::FIRE_SPEED:
      imageKey = "powerup-bullet-speed";
      break;
    case PowerupType::FIRE_DELAY:
      imageKey = "powerup-bullet-delay";
      break;
    default:
      break;
  }
}

void Powerup::draw(BITMAP* buffer) const {
  BITMAP* image = ImageRegistry::getImage(imageKey);
  draw_sprite(buffer, image, x, y);
}
