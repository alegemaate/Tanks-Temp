#include "player-tank.h"

#include <vector>

#include "../system/ImageRegistry.h"

// Init
PlayerTank::PlayerTank(World* world,
                       float x,
                       float y,
                       int health,
                       int fireSpeed,
                       int fireDelay,
                       float speed)
    : Tank(world, x, y, health, fireSpeed, fireDelay, speed) {
  this->image_treads = ImageRegistry::getImage("tank-treads");
  this->image_hurt = ImageRegistry::getImage("tank-dead");
  this->image_top = ImageRegistry::getImage("tank-turret-green");
  this->image_base = ImageRegistry::getImage("tank-base-green");

  this->width = image_base->w;
  this->height = image_base->h;
}

// Update
void PlayerTank::update() {
  Tank::update();

  if (dead) {
    return;
  }

  // Shoot
  rotation_turret = find_angle(SCREEN_W / 2, SCREEN_H / 2, mouse_x, mouse_y);

  if (joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0) {
    rotation_turret =
        find_angle(getCenterX() - 2, getCenterY() - 2,
                   (joy[0].stick[0].axis[0].pos) + (getCenterX() - 2),
                   (joy[0].stick[0].axis[1].pos) + (getCenterY() - 2));
  }

  if (key[KEY_SPACE] || mouse_b & 1 || joy[0].button[1].b) {
    shoot(rotation_turret, getCenterX() - 2, getCenterY() - 2);
  }

  // Rotate with keys
  if (key[KEY_A] || key[KEY_LEFT]) {
    rotation_body -= 0.03;
  }

  if (key[KEY_D] || key[KEY_RIGHT]) {
    rotation_body += 0.03;
  }

  // Drive
  if (mouse_b & 2) {
    rotation_body = rotation_turret;
  } else if (joy[0].button[0].b) {
    rotation_body = find_angle(getCenterX(), getCenterY(),
                               (joy[0].stick[0].axis[0].pos) + (getCenterX()),
                               (joy[0].stick[0].axis[1].pos) + (getCenterY()));
  }

  drive(rotation_body);

  accelerate(mouse_b & 2 || joy[0].button[0].b || key[KEY_W] || key[KEY_UP]);
}

// Feed AI player positions
void Tank::process_enemies(std::vector<Tank*>* tempOtherTanks) {
  otherTanks = tempOtherTanks;
}
