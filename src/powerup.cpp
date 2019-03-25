#include "powerup.h"
#include "tools.h"

BITMAP* powerup::images[4] = { nullptr };

powerup::powerup(world *wrld, int x, int y, int type) :
  Entity(wrld, x, y) {

  this -> image = nullptr;
  SetType(type);
}

powerup::~powerup(){

}

void powerup::SetType(int type) {
  // Ensure images are loaded
  if (images[0] == nullptr) {
    images[0] = load_bitmap_ex( "images/powerup_health.png");
    images[1] = load_bitmap_ex( "images/powerup_tank_speed.png");
    images[2] = load_bitmap_ex( "images/powerup_bullet_speed.png");
    images[3] = load_bitmap_ex( "images/powerup_bullet_delay.png");
  }

  // Set type
  this -> type = type;

  // Select type
  switch (type) {
    case POWERUP_HEALTH:
    {
      image = images[0];
      break;
    }
    case POWERUP_TANK_SPEED:
    {
      image = images[1];
      break;
    }
    case POWERUP_BULLET_SPEED:
    {
      image = images[2];
      break;
    }
    case POWERUP_BULLET_RATE:
    {
      image = images[3];
      break;
    }
  }

  if (image)
    SetDimensions(vec2<int>(image -> w, image -> h));
}

void powerup::Update() {

}

void powerup::Draw(BITMAP *buffer) {
  if (image)
    draw_sprite(buffer, image, GetX(), GetY());
}
