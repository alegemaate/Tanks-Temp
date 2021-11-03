#include "player-tank.h"

#include "../system/ImageRegistry.h"

// Init
PlayerTank::PlayerTank(World* world,
                       int x,
                       int y,
                       int hurtTime,
                       int health,
                       int fireSpeed,
                       int fireDelay,
                       float speed)
    : Tank(world, x, y, hurtTime, health, fireSpeed, fireDelay, speed) {
  this->image_treads = ImageRegistry::getImage("tank-treads");
  this->image_hurt = ImageRegistry::getImage("tank-dead");
  this->image_top = ImageRegistry::getImage("tank-turret-green");
  this->image_base = ImageRegistry::getImage("tank-base-green");

  this->width = image_base->w;
  this->height = image_base->h;
}

// Update
void PlayerTank::update() {
  if (!isDead()) {
    // Shoot
    rotation_radians_turret =
        find_angle(SCREEN_W / 2, SCREEN_H / 2, mouse_x,
                   mouse_y);  // find_angle( x + 25, y + 25, mouse_x, mouse_y);
    if (joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0)
      rotation_radians_turret =
          find_angle(x + width / 2 - 2, y + height / 2 - 2,
                     (joy[0].stick[0].axis[0].pos) + (x + 25),
                     (joy[0].stick[0].axis[1].pos) + (y + 25));
    rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

    if (key[KEY_SPACE] || mouse_b & 1 || joy[0].button[1].b) {
      shoot(rotation_radians_turret, x + width / 2 - 2, y + height / 2 - 2);
    }

    // Rotate with keys
    if (key[KEY_A] || key[KEY_LEFT]) {
      rotation_radians_body -= 0.03;
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
    }
    if (key[KEY_D] || key[KEY_RIGHT]) {
      rotation_radians_body += 0.03;
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
    }

    // Drive
    drive(rotation_radians_body);

    if (mouse_b & 2 || joy[0].button[0].b || key[KEY_W] || key[KEY_UP]) {
      if (mouse_b & 2) {
        rotation_radians_body =
            find_angle(SCREEN_W / 2, SCREEN_H / 2, mouse_x,
                       mouse_y);  // find_angle( x + width/2, y + height/2,
                                  // mouse_x, mouse_y);
      } else if (joy[0].button[0].b) {
        rotation_radians_body =
            find_angle(x + width / 2, y + height / 2,
                       (joy[0].stick[0].axis[0].pos) + (x + width / 2),
                       (joy[0].stick[0].axis[1].pos) + (y + height / 2));
      }

      rotation_allegro_body = rotation_radians_body * 40.5845104792;

      // Accelerate
      if (speed == 0)
        speed = 0.2;
      else if (speed < max_speed)
        speed *= (max_speed * 1.03);
      else
        speed = max_speed;
    } else {
      // Decelerate
      if (speed > 0.1)
        speed *= 0.95;
      else
        speed = 0;
    }
  }

  // Update bullets
  update_timers();
  update_bullets();
}

// Feed AI player positions
void Tank::process_enemies(std::vector<Tank*>* tempOtherTanks) {
  otherTanks = tempOtherTanks;
}
