#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../system/ImageRegistry.hpp"
#include "../util/Random.hpp"

unsigned char Tank::num_bullet_bounces = 0;
asw::Sample Tank::sample_shot;

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
      image_base(nullptr),
      image_hurt(nullptr),
      image_top(nullptr),
      image_treads(nullptr),
      worldPointer(worldPointer) {
  // Map size
  auto screenSize = asw::display::getSize();
  map_width = screenSize.x;
  map_height = screenSize.y;

  if (Tank::sample_shot == nullptr) {
    Tank::sample_shot = asw::assets::loadSample("assets/sfx/fire.wav");
  }
}

// Check dead
bool Tank::isDead() {
  return dead;
}

// Explode
void Tank::explode() {
  for (int i = 0; i < 200; i++) {
    auto particle = std::make_shared<Particle>(
        getCenterX(), getCenterY(),
        asw::util::makeColor(255, Random::random(0, 255), 0), -10.0f, 10.0f,
        -10.0f, 10.0f, 2, ParticleType::SQUARE, 20, ParticleBehaviour::EXPLODE);
    worldPointer->addParticle(particle);
  }
}

void Tank::accelerate(bool moving, const float deltaTime) {
  if (moving) {
    if (speed < 0.1f) {
      speed = 0.2f;
    } else if (speed < max_speed) {
      speed *= (max_speed * 1.03f) * (deltaTime / 8.0f);
    } else {
      speed = max_speed;
    }
  } else {
    if (speed > 0.1f) {
      speed /= 1.05f * (deltaTime / 8.0f);
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
void Tank::checkCollision(std::vector<Bullet*>* enemyBullets,
                          const float deltaTime) {
  for (auto* const& bullet : *enemyBullets) {
    if (collisionAny(
            x, x + 50, bullet->getX(),
            bullet->getX() + bullet->getXVelocity() * (deltaTime / 8.0f), y,
            y + 50, bullet->getY(),
            bullet->getY() + bullet->getYVelocity() * (deltaTime / 8.0f))) {
      health -= 10;
      bullet->destroy();
    }
  }
}

void Tank::checkCollision(const std::vector<std::unique_ptr<Barrier>>& barriers,
                          const float deltaTime) {
  float delta_speed = speed * (deltaTime / 8.0f);
  float guess_vector_x = -delta_speed * cosf(rotation_body);
  float guess_vector_y = -delta_speed * sinf(rotation_body);

  canMoveX = true;
  canMoveY = true;

  for (auto const& barrier : barriers) {
    if (collisionAny(x + 2 + guess_vector_x, x + width - 2 + guess_vector_x,
                     barrier->getPosition().x,
                     barrier->getPosition().x + barrier->getWidth(), y + 2,
                     y + height - 2, barrier->getPosition().y,
                     barrier->getPosition().y + barrier->getHeight())) {
      canMoveX = false;
    }
    if (collisionAny(x + 2, x + width - 2, barrier->getPosition().x,
                     barrier->getPosition().x + barrier->getWidth(),
                     y + 2 + guess_vector_y, y + height - 2 + guess_vector_y,
                     barrier->getPosition().y,
                     barrier->getPosition().y + barrier->getHeight())) {
      canMoveY = false;
    }
  }
}

void Tank::checkCollision(
    const std::vector<std::unique_ptr<PowerUp>>& power_ups,
    const float deltaTime) {
  for (auto const& power_up : power_ups) {
    if (collisionAny(x, x + 50, power_up->getX(),
                     power_up->getX() + power_up->getWidth(), y, y + 50,
                     power_up->getY(),
                     power_up->getY() + power_up->getHeight())) {
      pickupPowerUp(power_up->getType());
      power_up->pickup();
    }
  }
}

// Move around
void Tank::drive(float rotation, const float deltaTime) {
  float deltaSpeed = speed * (deltaTime / 8.0f);

  if (canMoveX) {
    x += -deltaSpeed * cosf(rotation);
  }
  if (canMoveY) {
    y += -deltaSpeed * sinf(rotation);
  }
}

// Update bullets
void Tank::update_bullets(const float deltaTime) {
  // Update bullets
  for (auto* const& bullet : bullets) {
    bullet->update(deltaTime);
  }

  // Erase bullets
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](auto* const& bullet) { return bullet->getErase(); }),
      bullets.end());
}

// Shoot
void Tank::shoot(float rotation, float targetX, float targetY) {
  if (bullet_delay > fire_delay_rate) {
    asw::sound::play(sample_shot, 255, 127, 0);

    auto* bullet = new Bullet(worldPointer, targetX, targetY, rotation,
                              fire_speed, 1 + num_bullet_bounces);
    bullets.push_back(bullet);
    bullet_delay -= fire_delay_rate;
  }
}

// Update
void Tank::update(const double deltaTime) {
  // Just died
  if (!dead && (health < 1)) {
    explode();
    asw::sound::play(sample_shot, 255, 127, 0);
    dead = true;
  }

  bullet_delay += deltaTime;

  update_bullets(deltaTime);
}

// Draw bullets
void Tank::drawBullets() const {
  for (auto* const& bullet : bullets) {
    bullet->draw();
  }
}

// Draw Tank
void Tank::drawTankBase() {
  // Hurt image for player
  if (dead) {
    asw::draw::rotateSprite(image_hurt, x, y, rad_to_deg(rotation_body));
  } else {
    asw::draw::rotateSprite(image_base, x, y, rad_to_deg(rotation_body));
  }
}

// Draw turret
void Tank::drawTankTurret() {
  asw::draw::rotateSprite(image_top, x, y, rad_to_deg(rotation_turret));
}

// Draw health
void Tank::drawHealthBar(float x,
                         float y,
                         int width,
                         int height,
                         int border) const {
  float healthPercent =
      static_cast<float>(health) / static_cast<float>(initialHealth);

  asw::draw::rectFill(x, y, width, height, asw::util::makeColor(0, 0, 0));
  asw::draw::rectFill(x + border, y + border, width - border, height - border,
                      asw::util::makeColor(255, 0, 0));
  asw::draw::rectFill(x + border, y + border, (healthPercent * width) - border,
                      height - border, asw::util::makeColor(0, 255, 0));
}

// Draw
void Tank::draw() {
  // Tank
  drawTankBase();

  // Bullets
  drawBullets();

  // Turret
  if (!isDead()) {
    drawTankTurret();

    // Health bar
    if (health < initialHealth) {
      drawHealthBar(x - 5, y - 10, 50, 6, 1);
    }
  }
}

// Put decals
void Tank::putDecal() {
  if (!dead && speed > 0) {
    asw::draw::rotateSprite(image_treads, getCenterX(), y,
                            rad_to_deg(rotation_turret));
  }
}

// Power ups
void Tank::pickupPowerUp(PowerUpType type) {
  switch (type) {
    case PowerUpType::HEALTH:
      health += 10;
      if (health > 100) {
        health = 100;
      }
      break;
    case PowerUpType::SPEED:
      max_speed += 0.5f;
      break;
    case PowerUpType::FIRE_SPEED:
      fire_speed += 1;
      break;
    case PowerUpType::FIRE_DELAY:
      fire_delay_rate -= 1;
      if (fire_delay_rate < 0) {
        fire_delay_rate = 0;
      }
      break;
    default:
      break;
  }
}
