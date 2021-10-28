#include "tank.h"

#include <cmath>

unsigned char Tank::num_bullet_bounces = 0;

/*****************
  General Tank
*****************/
Tank::Tank(world* worldPointer,
           int x,
           int y,
           int hurtTimer,
           int health,
           int fireSpeed,
           int fireDelay,
           float speed,
           BITMAP* baseImage,
           BITMAP* turretImage,
           BITMAP* hurtImage,
           BITMAP* treadsImage)
    : x(x),
      y(y),
      hurt_timer(hurtTimer),
      health(health),
      initialHealth(health),
      fire_speed(fireSpeed),
      fire_delay_rate(fireDelay),
      max_speed(speed),
      speed(0),
      image_base(baseImage),
      image_hurt(hurtImage),
      image_top(turretImage),
      image_treads(treadsImage),
      worldPointer(worldPointer),
      dead(false),
      rotation_radians_body(0),
      rotation_allegro_body(0),
      rotation_radians_turret(0),
      rotation_allegro_turret(0) {
  width = image_base->w;
  height = image_base->h;

  // Map size
  map_width = SCREEN_W;
  map_height = SCREEN_H;

  bullet_delay = 0;

  vector_x = 0;
  vector_y = 0;

  canMoveX = true;
  canMoveY = true;

  if (image_base->w < 1)
    abort_on_error(
        "Cannot find tank base\nPlease check your files and try again");
  if (image_hurt->w < 1)
    abort_on_error(
        "Cannot find tank hurt\nPlease check your files and try again");
  if (image_top->w < 1)
    abort_on_error(
        "Cannot find tank turret\nPlease check your files and try again");
  if (image_treads->w < 1)
    abort_on_error(
        "Cannot find tank treads\nPlease check your files and try again");

  sample_shot = load_sample_ex("sfx/fire.wav");
}

// Delete
Tank::~Tank() {
  //  destroy_sample(sample_shot);
}

// Check dead
bool Tank::isDead() {
  // Just died
  if (!dead && (health < 1)) {
    explode(x + 25, y + 25, 10, 200, 20);
    play_sample(sample_shot, 255, 127, 500, 0);
  }

  // Set dead
  dead = (health < 1);

  return dead;
}

// Explode
void Tank::explode(int x, int y, int velocity, int amount, int life) {
  for (int i = 0; i < amount; i++) {
    Particle* particle =
        new Particle(x, y, makecol(255, random(0, 255), 0), -velocity, velocity,
                     -velocity, velocity, 1, CIRCLE, life, EXPLODE);
    worldPointer->addParticle(particle);
  }
}

// Get bullets
std::vector<Bullet>* Tank::getBullets() {
  return &bullets;
}

// Check collision
void Tank::checkCollision(std::vector<Bullet>* bullets) {
  for (unsigned int i = 0; i < bullets->size(); i++) {
    if (collisionAny(x, x + 50, bullets->at(i).getX(),
                     bullets->at(i).getX() + bullets->at(i).getXVelocity(), y,
                     y + 50, bullets->at(i).getY(),
                     bullets->at(i).getY() + bullets->at(i).getYVelocity())) {
      health -= 10;
      bullets->at(i).destroy();
    }
  }
}

void Tank::checkCollision(std::vector<Barrier>* barriers) {
  float guess_vector_x = -speed * cos(rotation_radians_body);
  float guess_vector_y = -speed * sin(rotation_radians_body);

  canMoveX = true;
  canMoveY = true;

  for (unsigned int i = 0; i < barriers->size(); i++) {
    if (collisionAny(x + 2 + guess_vector_x, x + width - 2 + guess_vector_x,
                     barriers->at(i).getX(),
                     barriers->at(i).getX() + barriers->at(i).getWidth(), y + 2,
                     y + height - 2, barriers->at(i).getY(),
                     barriers->at(i).getY() + barriers->at(i).getHeight())) {
      canMoveX = false;
    }
    if (collisionAny(x + 2, x + width - 2, barriers->at(i).getX(),
                     barriers->at(i).getX() + barriers->at(i).getWidth(),
                     y + 2 + guess_vector_y, y + height - 2 + guess_vector_y,
                     barriers->at(i).getY(),
                     barriers->at(i).getY() + barriers->at(i).getHeight())) {
      canMoveY = false;
    }
  }
}
void Tank::checkCollision(std::vector<Powerup>* powerups) {
  for (unsigned int i = 0; i < powerups->size(); i++) {
    if (collisionAny(x, x + 50, powerups->at(i).getX(),
                     powerups->at(i).getX() + powerups->at(i).getWidth(), y,
                     y + 50, powerups->at(i).getY(),
                     powerups->at(i).getY() + powerups->at(i).getHeight())) {
      get_powerup(powerups->at(i).getType());
      powerups->at(i).pickup();
    }
  }
}

// Move around
void Tank::drive(float rotation) {
  if (canMoveX) {
    vector_x = -speed * cos(rotation);
    x += vector_x;
  }
  if (canMoveY) {
    vector_y = -speed * sin(rotation);
    y += vector_y;
  }
}

// Update timers
void Tank::update_timers() {
  // Change timers
  hurt_timer--;
  bullet_delay++;
}

// Update bullets
void Tank::update_bullets() {
  // Update bullets
  for (unsigned int i = 0; i < bullets.size(); i++) {
    bullets.at(i).update();
    if (bullets.at(i).getErase())
      bullets.erase(bullets.begin() + i);
  }

  // Erase bullets
  if (key[KEY_C] || joy[0].button[4].b) {
    bullets.clear();
  }
}

// Shoot
void Tank::shoot(float rotation, float x, float y) {
  if (bullet_delay > fire_delay_rate) {
    Bullet bullet(worldPointer, x, y, rotation, fire_speed,
                  1 + num_bullet_bounces, sample_shot);
    bullets.push_back(bullet);
    bullet_delay = 0;
  }
}

// Update
void Tank::update() {}

// Draw bullets
void Tank::drawBullets(BITMAP* buffer) {
  // Draw bullets
  for (unsigned int i = 0; i < bullets.size(); i++)
    bullets.at(i).draw(buffer);
}

// Draw Tank
void Tank::drawTankBase(BITMAP* buffer) {
  // Hurt image for player
  if (health <= 0) {
    rotate_sprite(buffer, image_hurt, x, y, itofix(rotation_allegro_body));
  } else {
    rotate_sprite(buffer, image_base, x, y, itofix(rotation_allegro_body));
  }
}

// Draw turret
void Tank::drawTankTurret(BITMAP* buffer) {
  // Turret
  rotate_sprite(buffer, image_top, x, y, itofix(rotation_allegro_turret));
}

// Draw health
void Tank::drawHealthBar(BITMAP* buffer,
                         int x,
                         int y,
                         int width,
                         int height,
                         int border) {
  // Health Bar
  rectfill(buffer, x, y, x + width, y + height, makecol(0, 0, 0));
  rectfill(buffer, x + border, y + border, x + width - border,
           y + height - border, makecol(255, 0, 0));
  rectfill(buffer, x + border, y + border,
           x + (((float)health / (float)initialHealth) * width) - border,
           y + height - border, makecol(0, 255, 0));
}

// Draw
void Tank::draw(BITMAP* buffer) {
  // Tank
  drawTankBase(buffer);

  // Bullets
  drawBullets(buffer);

  // Turret
  if (!isDead()) {
    drawTankTurret(buffer);

    // Health bar
    if (health < initialHealth)
      drawHealthBar(buffer, x - 5, y - 10, 50, 6, 1);
  }
}

// Put decals
void Tank::putDecal(BITMAP* buffer) {
  if (!dead && speed > 0)
    rotate_sprite(buffer, image_treads, x + width / 2, y,
                  itofix(rotation_allegro_body));
  else if (dead)
    drawTankBase(buffer);
}

// Health
void Tank::giveHealth(int healthAmount) {
  health += healthAmount;
  if (health > initialHealth)
    health = initialHealth;
}

// Powerups
void Tank::get_powerup(int powerup_id) {
  if (powerup_id == 0) {
    health += 10;
    if (health > 100)
      health = 100;
  } else if (powerup_id == 1) {
    max_speed += 0.5;
  } else if (powerup_id == 2) {
    fire_speed += 1;
  } else if (powerup_id == 3) {
    fire_delay_rate -= 1;
    if (fire_delay_rate < 0)
      fire_delay_rate = 0;
  }
}

/*****************
   Player Tank
*****************/
// Init
PlayerTank::PlayerTank(world* world,
                       int x,
                       int y,
                       int hurtTime,
                       int health,
                       int fireSpeed,
                       int fireDelay,
                       float speed,
                       BITMAP* baseImage,
                       BITMAP* turretImage,
                       BITMAP* hurtImage,
                       BITMAP* treadsImage)
    : Tank(world,
           x,
           y,
           hurtTime,
           health,
           fireSpeed,
           fireDelay,
           speed,
           baseImage,
           turretImage,
           hurtImage,
           treadsImage) {}

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

/*****************
    AI Tank
*****************/
// Init
AiTank::AiTank(world* world,
               int x,
               int y,
               int hurtTime,
               int health,
               int fireSpeed,
               int fireDelay,
               float speed,
               BITMAP* baseImage,
               BITMAP* turretImage,
               BITMAP* hurtImage,
               BITMAP* treadsImage)
    : Tank(world,
           x,
           y,
           hurtTime,
           health,
           fireSpeed,
           fireDelay,
           speed,
           baseImage,
           turretImage,
           hurtImage,
           treadsImage) {
  destination_x = x;
  destination_y = y;
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
