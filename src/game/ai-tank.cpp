#include "ai-tank.h"

#include "../system/ImageRegistry.h"

// Init
AiTank::AiTank(World* world,
               float x,
               float y,
               int health,
               int fireSpeed,
               int fireDelay,
               float speed,
               bool enemy)
    : Tank(world, x, y, health, fireSpeed, fireDelay, speed) {
  if (enemy) {
    this->image_treads = ImageRegistry::getImage("tank-treads");
    this->image_hurt = ImageRegistry::getImage("tank-dead");
    this->image_top = ImageRegistry::getImage("tank-turret-red");
    this->image_base = ImageRegistry::getImage("tank-base-red");
  } else {
    this->image_treads = ImageRegistry::getImage("tank-treads");
    this->image_hurt = ImageRegistry::getImage("tank-dead");
    this->image_top = ImageRegistry::getImage("tank-turret-blue");
    this->image_base = ImageRegistry::getImage("tank-base-blue");
  }

  this->destination_x = x;
  this->destination_y = y;

  this->width = image_base->w;
  this->height = image_base->h;
}

// Update
void AiTank::update() {
  Tank::update();

  if (dead) {
    return;
  }

  find_enemy_target();
  update_target();
  ai_drive();
}

void AiTank::find_enemy_target() {
  // Rotate turret (at closest enemy)
  float closest_enemy_x = destination_x;
  float closest_enemy_y = destination_y;
  bool found_enemy = false;

  for (auto const& tank : *otherTanks) {
    if (!tank->isDead()) {
      float enemy_x = tank->getX() + tank->getCenterX();
      float enemy_y = tank->getY() + tank->getCenterY();
      found_enemy = true;

      if (find_distance(x, y, enemy_x, enemy_y) <
          find_distance(x, y, closest_enemy_x, closest_enemy_y)) {
        closest_enemy_x = enemy_x;
        closest_enemy_y = enemy_y;
      }
    }
  }

  if (found_enemy) {
    rotation_turret =
        find_angle(x + 25, y + 25, closest_enemy_x, closest_enemy_y);

    // Shoot
    float distanceToEnemy =
        find_distance(x, y, closest_enemy_x, closest_enemy_y);

    if (random(0, 10) == 0 && distanceToEnemy < 500) {
      shoot(rotation_turret, x + 23, y + 23);
    }
  } else {
    rotation_turret = rotation_body;
  }
}

// Ai point choosing
void AiTank::update_target() {
  float distanceToTarget =
      find_distance(getCenterX(), getCenterY(), destination_x, destination_y);
  bool cantMove = !canMoveX && !canMoveY;

  if (distanceToTarget < 10 || cantMove) {
    destination_x = random(0, map_width);
    destination_y = random(0, map_height);
  }
}

void AiTank::ai_drive() {
  rotation_body = find_angle(x + 25, y + 25, destination_x, destination_y);
  accelerate(true);
  drive(rotation_body);
}
