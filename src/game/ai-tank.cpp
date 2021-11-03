#include "ai-tank.h"

#include "../system/ImageRegistry.h"

// Init
AiTank::AiTank(World* world,
               int x,
               int y,
               int hurtTime,
               int health,
               int fireSpeed,
               int fireDelay,
               float speed,
               bool enemy)
    : Tank(world, x, y, hurtTime, health, fireSpeed, fireDelay, speed) {
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
  if (!isDead()) {
    // Rotate turret (at closest enemy)
    int best_enemy_x = destination_x;
    int best_enemy_y = destination_y;
    int target_enemy_index = 0;

    // Find nearest enemy and assign to target_enemy_index
    if (otherTanks->size() > 0) {
      int temp_enemy_x, temp_enemy_y;

      for (unsigned int i = 0; i < otherTanks->size(); i++) {
        temp_enemy_x =
            otherTanks->at(i)->getX() + otherTanks->at(i)->getWidth() / 2;
        temp_enemy_y =
            otherTanks->at(i)->getY() + otherTanks->at(i)->getHeight() / 2;

        best_enemy_x = otherTanks->at(target_enemy_index)->getX() +
                       otherTanks->at(target_enemy_index)->getWidth() / 2;
        best_enemy_y = otherTanks->at(target_enemy_index)->getY() +
                       otherTanks->at(target_enemy_index)->getHeight() / 2;

        if (find_distance(x, y, temp_enemy_x, temp_enemy_y) <
            find_distance(x, y, best_enemy_x, best_enemy_y)) {
          target_enemy_index = i;
          best_enemy_x = otherTanks->at(target_enemy_index)->getX() +
                         otherTanks->at(target_enemy_index)->getWidth() / 2;
          best_enemy_y = otherTanks->at(target_enemy_index)->getY() +
                         otherTanks->at(target_enemy_index)->getHeight() / 2;
        }
      }

      rotation_radians_turret = find_angle(x + 25, y + 25, best_enemy_x,
                                           best_enemy_y);  // randomf(-0.1,0.1);
      rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

      // Shoot
      if (random(0, 10) == 0 &&
          find_distance(x, y, best_enemy_x, best_enemy_y) < 500) {
        shoot(rotation_radians_turret, x + 23, y + 23);
      }
    } else {
      rotation_radians_turret = rotation_radians_body;
      rotation_allegro_turret = rotation_allegro_body;
    }

    // Path
    update_target();

    // Drive
    if (random(0, 100)) {
      rotation_radians_body =
          find_angle(x + 25, y + 25, destination_x, destination_y);
      rotation_allegro_body = rotation_radians_body * 40.5845104792;

      // Accelerate
      if (speed == 0)
        speed = 0.2;
      else if (speed < 1)
        speed *= 1.03;
      else
        speed = 1;

      drive(rotation_radians_body);
    } else {
      speed = 0;
    }
  }

  // Update bullets
  update_timers();
  update_bullets();
}

// Ai point choosing
void AiTank::update_target() {
  if (find_distance(x + 25, y + 25, destination_x, destination_y) < 10 ||
      (canMoveX == false && canMoveY == false)) {
    destination_x = random(0, map_width);
    destination_y = random(0, map_height);
  }
}
