#include "tank.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../system/ImageRegistry.h"

unsigned char Tank::num_bullet_bounces = 0;

Tank::Tank(World* worldPointer,
           float x,
           float y,
           int health,
           int fireSpeed,
           int fireDelay,
           float speed)
    : x(x),
      y(y),
      health(health),
      initialHealth(health),
      fire_speed(fireSpeed),
      fire_delay_rate(fireDelay),
      max_speed(speed),
      speed(0),
      worldPointer(worldPointer),
      dead(false),
      rotation_body(0),
      rotation_turret(0),
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

void Tank::accelerate(bool moving) {
  if (moving) {
    if (speed == 0) {
      speed = 0.2;
    } else if (speed < max_speed) {
      speed *= (max_speed * 1.03);
    } else {
      speed = max_speed;
    }
  } else {
    if (speed > 0.1) {
      speed *= 0.95;
    } else {
      speed = 0;
    }
  }
}

// Get bullets
std::vector<Bullet*>* Tank::getBullets() {
  return &bullets;
}

// Check collision
void Tank::checkCollision(std::vector<Bullet*>* bullets) {
  for (auto* const& bullet : *bullets) {
    if (collisionAny(x, x + 50, bullet->getX(),
                     bullet->getX() + bullet->getXVelocity(), y, y + 50,
                     bullet->getY(), bullet->getY() + bullet->getYVelocity())) {
      health -= 10;
      bullet->destroy();
    }
  }
}

void Tank::checkCollision(
    const std::vector<std::unique_ptr<Barrier>>& barriers) {
  float guess_vector_x = -speed * cos(rotation_body);
  float guess_vector_y = -speed * sin(rotation_body);

  canMoveX = true;
  canMoveY = true;

  for (auto const& barrier : barriers) {
    if (collisionAny(x + 2 + guess_vector_x, x + width - 2 + guess_vector_x,
                     barrier->position.x,
                     barrier->position.x + barrier->getWidth(), y + 2,
                     y + height - 2, barrier->position.y,
                     barrier->position.y + barrier->getHeight())) {
      canMoveX = false;
    }
    if (collisionAny(x + 2, x + width - 2, barrier->position.x,
                     barrier->position.x + barrier->getWidth(),
                     y + 2 + guess_vector_y, y + height - 2 + guess_vector_y,
                     barrier->position.y,
                     barrier->position.y + barrier->getHeight())) {
      canMoveY = false;
    }
  }
}

void Tank::checkCollision(
    const std::vector<std::unique_ptr<Powerup>>& powerups) {
  for (auto const& powerup : powerups) {
    if (collisionAny(x, x + 50, powerup->getX(),
                     powerup->getX() + powerup->getWidth(), y, y + 50,
                     powerup->getY(), powerup->getY() + powerup->getHeight())) {
      pickupPowerup(powerup->getType());
      powerup->pickup();
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

// Update bullets
void Tank::update_bullets() {
  // Update bullets
  for (auto* const& bullet : bullets) {
    bullet->update();
  }

  // Erase bullets
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](auto* const& bullet) { return bullet->getErase(); }),
      bullets.end());
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
void Tank::update() {
  // Just died
  if (!dead && (health < 1)) {
    explode(x + 25, y + 25, 10, 200, 20);
    play_sample(sample_shot, 255, 127, 500, 0);
    dead = true;
  }

  bullet_delay++;

  update_bullets();
}

// Draw bullets
void Tank::drawBullets(BITMAP* buffer) {
  for (auto* const& bullet : bullets) {
    bullet->draw(buffer);
  }
}

// Draw Tank
void Tank::drawTankBase(BITMAP* buffer) {
  // Hurt image for player
  if (dead) {
    rotate_sprite(buffer, image_hurt, x, y, radToFix(rotation_body));
  } else {
    rotate_sprite(buffer, image_base, x, y, radToFix(rotation_body));
  }
}

// Draw turret
void Tank::drawTankTurret(BITMAP* buffer) {
  rotate_sprite(buffer, image_top, x, y, radToFix(rotation_turret));
}

// Draw health
void Tank::drawHealthBar(BITMAP* buffer,
                         float x,
                         float y,
                         int width,
                         int height,
                         int border) {
  float healthPercent =
      static_cast<float>(health) / static_cast<float>(initialHealth);

  rectfill(buffer, x, y, x + width, y + height, makecol(0, 0, 0));
  rectfill(buffer, x + border, y + border, x + width - border,
           y + height - border, makecol(255, 0, 0));
  rectfill(buffer, x + border, y + border, x + (healthPercent * width) - border,
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
    if (health < initialHealth) {
      drawHealthBar(buffer, x - 5, y - 10, 50, 6, 1);
    }
  }
}

// Put decals
void Tank::putDecal(BITMAP* buffer) {
  if (!dead && speed > 0) {
    rotate_sprite(buffer, image_treads, getCenterX(), y,
                  radToFix(rotation_body));
  }
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
