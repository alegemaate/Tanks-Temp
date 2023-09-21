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

  if (asw::input::isKeyDown(asw::input::Key::SPACE) ||
      asw::input::isButtonDown(
          asw::input::MouseButton::LEFT)) {  // || joy[0].button[1].b TODO
    shoot(rotation_turret, getCenterX() - 2, getCenterY() - 2);
  }

  // Rotate with keys
  if (asw::input::isKeyDown(asw::input::Key::A) ||
      asw::input::isKeyDown(asw::input::Key::LEFT)) {
    rotation_body -= 0.03f * (deltaTime / 8.0f);
  }

  if (asw::input::isKeyDown(asw::input::Key::D) ||
      asw::input::isKeyDown(asw::input::Key::RIGHT)) {
    rotation_body += 0.03f * (deltaTime / 8.0f);
  }

  // Drive
  if (asw::input::isButtonDown(asw::input::MouseButton::RIGHT)) {
    rotation_body = rotation_turret;
  }
  // else if (joy[0].button[0].b) { TODO
  //   rotation_body = find_angle(
  //       getCenterX(), getCenterY(),
  //       static_cast<float>(joy[0].stick[0].axis[0].pos) + getCenterX(),
  //       static_cast<float>(joy[0].stick[0].axis[1].pos) + getCenterY());
  // }

  drive(rotation_body, deltaTime);

  accelerate(
      asw::input::isButtonDown(
          asw::input::MouseButton::RIGHT) || /* TODO joy[0].button[0].b || */
          asw::input::isKeyDown(asw::input::Key::W) ||
          asw::input::isKeyDown(asw::input::Key::UP),
      deltaTime);
}

// Feed AI player positions
void Tank::process_enemies(std::vector<Tank*>* tempOtherTanks) {
  otherTanks = tempOtherTanks;
}
