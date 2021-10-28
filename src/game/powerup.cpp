#include "powerup.h"

Powerup::Powerup() : Powerup(0, 0, 0, nullptr) {}

Powerup::Powerup(int x, int y, int type, BITMAP* image) {
  this->x = x;
  this->y = y;
  this->type = type;
  this->image = image;
  this->dead = false;
}

Powerup::~Powerup() {}

void Powerup::draw(BITMAP* tempBitmap) {
  draw_sprite(tempBitmap, image, x, y);
}
