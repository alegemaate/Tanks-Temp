#include "tank.h"

#include <cmath>

#include "../system/ImageRegistry.h"

unsigned char Tank::num_bullet_bounces = 0;

Tank::Tank(World* worldPointer,
           int x,
           int y,
           int hurtTimer,
           int health,
           int fireSpeed,
           int fireDelay,
           float speed)
    : x(x),
      y(y),
      hurt_timer(hurtTimer),
      health(health),
      initialHealth(health),
      fire_speed(fireSpeed),
      fire_delay_rate(fireDelay),
      max_speed(speed),
      speed(0),
      worldPointer(worldPointer),
      dead(false),
      rotation_radians_body(0),
      rotation_allegro_body(0),
      rotation_radians_turret(0),
      rotation_allegro_turret(0),
      width(0),
      height(0) {
  // Map size
  map_width = SCREEN_W;
  map_height = SCREEN_H;

  bullet_delay = 0;

  vector_x = 0;
  vector_y = 0;

  canMoveX = true;
  canMoveY = true;

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
std::vector<Bullet*>* Tank::getBullets() {
  return &bullets;
}

// Check collision
void Tank::checkCollision(std::vector<Bullet*>* bullets) {
  for (unsigned int i = 0; i < bullets->size(); i++) {
    if (collisionAny(x, x + 50, bullets->at(i)->getX(),
                     bullets->at(i)->getX() + bullets->at(i)->getXVelocity(), y,
                     y + 50, bullets->at(i)->getY(),
                     bullets->at(i)->getY() + bullets->at(i)->getYVelocity())) {
      health -= 10;
      bullets->at(i)->destroy();
    }
  }
}

void Tank::checkCollision(std::vector<Barrier*>* barriers) {
  float guess_vector_x = -speed * cos(rotation_radians_body);
  float guess_vector_y = -speed * sin(rotation_radians_body);

  canMoveX = true;
  canMoveY = true;

  for (unsigned int i = 0; i < barriers->size(); i++) {
    if (collisionAny(
            x + 2 + guess_vector_x, x + width - 2 + guess_vector_x,
            barriers->at(i)->position.x,
            barriers->at(i)->position.x + barriers->at(i)->getWidth(), y + 2,
            y + height - 2, barriers->at(i)->position.y,
            barriers->at(i)->position.y + barriers->at(i)->getHeight())) {
      canMoveX = false;
    }
    if (collisionAny(
            x + 2, x + width - 2, barriers->at(i)->position.x,
            barriers->at(i)->position.x + barriers->at(i)->getWidth(),
            y + 2 + guess_vector_y, y + height - 2 + guess_vector_y,
            barriers->at(i)->position.y,
            barriers->at(i)->position.y + barriers->at(i)->getHeight())) {
      canMoveY = false;
    }
  }
}
void Tank::checkCollision(std::vector<Powerup*>* powerups) {
  for (unsigned int i = 0; i < powerups->size(); i++) {
    if (collisionAny(x, x + 50, powerups->at(i)->getX(),
                     powerups->at(i)->getX() + powerups->at(i)->getWidth(), y,
                     y + 50, powerups->at(i)->getY(),
                     powerups->at(i)->getY() + powerups->at(i)->getHeight())) {
      pickupPowerup(powerups->at(i)->getType());
      powerups->at(i)->pickup();
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
    bullets.at(i)->update();
    if (bullets.at(i)->getErase())
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
    Bullet* bullet = new Bullet(worldPointer, x, y, rotation, fire_speed,
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
    bullets.at(i)->draw(buffer);
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
void Tank::pickupPowerup(PowerupType type) {
  switch (type) {
    case PowerupType::Health:
      health += 10;
      if (health > 100)
        health = 100;
      break;
    case PowerupType::Speed:
      max_speed += 0.5;
      break;
    case PowerupType::FireSpeed:
      fire_speed += 1;
      break;
    case PowerupType::FireDelay:
      fire_delay_rate -= 1;
      if (fire_delay_rate < 0)
        fire_delay_rate = 0;
      break;
  }
}
