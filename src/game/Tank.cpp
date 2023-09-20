#include "Tank.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../system/ImageRegistry.hpp"
#include "../util/Random.hpp"

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
      image_base(nullptr),
      image_hurt(nullptr),
      image_top(nullptr),
      image_treads(nullptr),
      worldPointer(worldPointer) {
  // Map size
  auto screenSize = asw::display::getSize();
  map_width = screenSize.x;
  map_height = screenSize.y;

  sample_shot = asw::assets::loadSample("assets/sfx/fire.wav");
}

// Check dead
bool Tank::isDead() {
  return dead;
}

// Explode
void Tank::explode() {
  for (int i = 0; i < 200; i++) {
    auto* particle = new Particle(
        getCenterX(), getCenterY(),
        asw::util::makeColor(255, Random::random(0, 255), 0), -10.0f, 10.0f,
        -10.0f, 10.0f, 1, ParticleType::CIRCLE, 20, ParticleBehaviour::EXPLODE);
    worldPointer->addParticle(particle);
  }
}

void Tank::accelerate(bool moving) {
  if (moving) {
    if (speed < 0.1f) {
      speed = 0.2f;
    } else if (speed < max_speed) {
      speed *= (max_speed * 1.03f);
    } else {
      speed = max_speed;
    }
  } else {
    if (speed > 0.1f) {
      speed *= 0.95f;
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
void Tank::checkCollision(std::vector<Bullet*>* enemyBullets) {
  for (auto* const& bullet : *enemyBullets) {
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
  float guess_vector_x = -speed * cosf(rotation_body);
  float guess_vector_y = -speed * sinf(rotation_body);

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
    const std::vector<std::unique_ptr<PowerUp>>& power_ups) {
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
void Tank::drive(float rotation) {
  if (canMoveX) {
    vector_x = -speed * cosf(rotation);
    x += vector_x;
  }
  if (canMoveY) {
    vector_y = -speed * sinf(rotation);
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
void Tank::shoot(float rotation, float targetX, float targetY) {
  if (bullet_delay > fire_delay_rate) {
    asw::sound::play(sample_shot, 255, 127, 0);

    auto* bullet = new Bullet(worldPointer, targetX, targetY, rotation,
                              fire_speed, 1 + num_bullet_bounces);
    bullets.push_back(bullet);
    bullet_delay = 0;
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

  bullet_delay++;

  update_bullets();
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
    //   rotate_sprite(image_hurt, x, y, radToFix(rotation_body));

    SDL_Point size = asw::util::getTextureSize(image_hurt);
    SDL_Rect dest = {static_cast<int>(x), static_cast<int>(y), size.x, size.y};
    SDL_RenderCopyEx(asw::display::renderer, image_hurt.get(), nullptr, &dest,
                     rad_to_deg(rotation_body), nullptr, SDL_FLIP_NONE);
  } else {
    // rotate_sprite(image_base, x, y, radToFix(rotation_body));

    SDL_Point size = asw::util::getTextureSize(image_base);
    SDL_Rect dest = {static_cast<int>(x), static_cast<int>(y), size.x, size.y};
    SDL_RenderCopyEx(asw::display::renderer, image_base.get(), nullptr, &dest,
                     rad_to_deg(rotation_body), nullptr, SDL_FLIP_NONE);
  }
}

// Draw turret
void Tank::drawTankTurret() {
  // rotate_sprite(image_top, x, y, radToFix(rotation_turret)); TODO

  SDL_Point size = asw::util::getTextureSize(image_top);
  SDL_Rect dest = {static_cast<int>(x), static_cast<int>(y), size.x, size.y};
  SDL_RenderCopyEx(asw::display::renderer, image_top.get(), nullptr, &dest,
                   rad_to_deg(rotation_turret), nullptr, SDL_FLIP_NONE);
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
    // rotate_sprite(image_treads, getCenterX(), y, radToFix(rotation_body));
    SDL_Point size = asw::util::getTextureSize(image_treads);
    SDL_Rect dest = {static_cast<int>(getCenterX()), static_cast<int>(y),
                     size.x, size.y};
    SDL_RenderCopyEx(asw::display::renderer, image_treads.get(), nullptr, &dest,
                     rad_to_deg(rotation_body), nullptr, SDL_FLIP_NONE);
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
