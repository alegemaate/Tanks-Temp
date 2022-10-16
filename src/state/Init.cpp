#include "Init.hpp"

#include "../system/ImageRegistry.hpp"
#include "StateEngine.hpp"

void Init::update(const double deltaTime) {
  // Load images
  ImageRegistry::loadImage("menu-background", "images/menu.png");
  ImageRegistry::loadImage("game-background", "images/background.png");
  ImageRegistry::loadImage("cursor", "images/cursor.png");
  ImageRegistry::loadImage("block-box", "images/block_box_1.png");
  ImageRegistry::loadImage("block-stone", "images/block_stone_1.png");
  ImageRegistry::loadImage("power-up-health", "images/powerup_health.png");
  ImageRegistry::loadImage("power-up-tank-speed",
                           "images/powerup_tank_speed.png");
  ImageRegistry::loadImage("power-up-bullet-speed",
                           "images/powerup_bullet_speed.png");
  ImageRegistry::loadImage("power-up-bullet-delay",
                           "images/powerup_bullet_delay.png");
  ImageRegistry::loadImage("tank-treads", "images/tank_treads.png");
  ImageRegistry::loadImage("tank-dead", "images/tank_dead.png");
  ImageRegistry::loadImage("tank-turret-green", "images/tank_turret_green.png");
  ImageRegistry::loadImage("tank-base-green", "images/tank_base_green.png");
  ImageRegistry::loadImage("tank-turret-red", "images/tank_turret_red.png");
  ImageRegistry::loadImage("tank-base-red", "images/tank_base_red.png");
  ImageRegistry::loadImage("tank-turret-blue", "images/tank_turret_blue.png");
  ImageRegistry::loadImage("tank-base-blue", "images/tank_base_blue.png");

  // Goto menu
  StateEngine::setNextState(StateId::STATE_MENU);
}
