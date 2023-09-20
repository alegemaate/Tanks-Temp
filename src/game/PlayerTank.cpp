#include "PlayerTank.hpp"

#include <vector>

#include "../system/ImageRegistry.hpp"

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

  auto imageSize = asw::util::getTextureSize(image_base);
  this->width = static_cast<float>(imageSize.x);
  this->height = static_cast<float>(imageSize.y);
}

// Update
void PlayerTank::update(const double deltaTime) {
  Tank::update(deltaTime);

  if (dead) {
    return;
  }

  // Screen size
  auto screenSize = asw::display::getSize();

  // Shoot
  rotation_turret = find_angle(static_cast<float>(screenSize.x) / 2.0f,
                               static_cast<float>(screenSize.y) / 2.0f,
                               static_cast<float>(asw::input::mouse.x),
                               static_cast<float>(asw::input::mouse.y));

  // if (joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0) {
  // TODO
  //   rotation_turret = find_angle(
  //       getCenterX() - 2.0f, getCenterY() - 2.0f,
  //       static_cast<float>(joy[0].stick[0].axis[0].pos) + (getCenterX()
  //       - 2.0f), static_cast<float>(joy[0].stick[0].axis[1].pos) +
  //           (getCenterY() - 2.0f));
  // }

  if (asw::input::keyboard.down[SDL_SCANCODE_SPACE] ||
      asw::input::mouse.down[1]) {  // || joy[0].button[1].b TODO
    shoot(rotation_turret, getCenterX() - 2, getCenterY() - 2);
  }

  // Rotate with keys
  if (asw::input::keyboard.down[SDL_SCANCODE_A] ||
      asw::input::keyboard.down[SDL_SCANCODE_LEFT]) {
    rotation_body -= 0.03f;
  }

  if (asw::input::keyboard.down[SDL_SCANCODE_D] ||
      asw::input::keyboard.down[SDL_SCANCODE_RIGHT]) {
    rotation_body += 0.03f;
  }

  // Drive
  if (asw::input::mouse.down[3]) {
    rotation_body = rotation_turret;
  }
  // else if (joy[0].button[0].b) { TODO
  //   rotation_body = find_angle(
  //       getCenterX(), getCenterY(),
  //       static_cast<float>(joy[0].stick[0].axis[0].pos) + getCenterX(),
  //       static_cast<float>(joy[0].stick[0].axis[1].pos) + getCenterY());
  // }

  drive(rotation_body);

  accelerate(asw::input::mouse.down[3] || /* TODO joy[0].button[0].b || */
             asw::input::keyboard.down[SDL_SCANCODE_W] ||
             asw::input::keyboard.down[SDL_SCANCODE_UP]);
}

// Feed AI player positions
void Tank::process_enemies(std::vector<Tank*>* tempOtherTanks) {
  otherTanks = tempOtherTanks;
}
