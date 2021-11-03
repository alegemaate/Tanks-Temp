#include "init.h"

#include "../system/ImageRegistry.h"

// Init state (and game)
Init::Init() {
  // Load images
  ImageRegistry::loadImage("menu-background", "images/menu.png");
  ImageRegistry::loadImage("game-background", "images/background.png");
  ImageRegistry::loadImage("cursor", "images/cursor.png");
  ImageRegistry::loadImage("block-box", "images/block_box_1.png");
  ImageRegistry::loadImage("block-stone", "images/block_stone_1.png");
  ImageRegistry::loadImage("powerup-health", "images/powerup_health.png");
  ImageRegistry::loadImage("powerup-tank-speed",
                           "images/powerup_tank_speed.png");
  ImageRegistry::loadImage("powerup-bullet-speed",
                           "images/powerup_bullet_speed.png");
  ImageRegistry::loadImage("powerup-bullet-delay",
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
  set_next_state(STATE_MENU);
}
