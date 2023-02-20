#include "Particle.hpp"
#include "../util/Random.hpp"

// Constructor
Particle::Particle(float x,
                   float y,
                   int color,
                   float xVelocityMin,
                   float xVelocityMax,
                   float yVelocityMin,
                   float yVelocityMax,
                   int size,
                   ParticleType type,
                   int life,
                   ParticleBehaviour behaviour)
    : x(x),
      y(y),
      color(color),
      size(size),
      type(type),
      life(life),
      behaviour(behaviour) {
  this->xVelocity = Random::randomFloat(xVelocityMin, xVelocityMax);
  this->yVelocity = Random::randomFloat(yVelocityMin, yVelocityMax);

  // No unmoving
  if (xVelocity < 0.1f && xVelocity > -0.1f) {
    this->xVelocity = 0.1f;
  }
  if (yVelocity < 0.1f && yVelocity > -0.1f) {
    this->yVelocity = 0.1f;
  }
}

// Logic
void Particle::logic() {
  // Behaviour
  if (behaviour == ParticleBehaviour::EXPLODE) {
    x += xVelocity;
    y += yVelocity;
    xVelocity -= xVelocity / 10;
    yVelocity -= yVelocity / 10;
  } else {
    x += Random::randomFloat(-xVelocity, xVelocity);
    y += Random::randomFloat(-yVelocity, yVelocity);
  }

  // Die
  if (Random::random(0, life) == 0) {
    dead = true;
  }
}

// Check death
bool Particle::getDead() const {
  return dead;
}

// Draw
void Particle::draw(BITMAP* buffer) const {
  switch (type) {
    case ParticleType::PIXEL:
      putpixel(buffer, static_cast<int>(x), static_cast<int>(y), color);
      break;
    case ParticleType::SQUARE:
      rectfill(buffer, static_cast<int>(x), static_cast<int>(y),
               static_cast<int>(x) + size, static_cast<int>(y) + size, color);
      break;
    case ParticleType::CIRCLE:
      circlefill(buffer, static_cast<int>(x), static_cast<int>(y), size, color);
      break;
    case ParticleType::RANDOM:
      drawRandom(buffer);
      break;
    default:
      break;
  }
}

void Particle::drawRandom(BITMAP* buffer) const {
  switch (Random::random(0, 3)) {
    case 0:
      putpixel(buffer, static_cast<int>(x), static_cast<int>(y), color);
      break;
    case 1:
      circlefill(buffer, static_cast<int>(x), static_cast<int>(y), size, color);
      break;
    case 2:
      rectfill(buffer, static_cast<int>(x), static_cast<int>(y),
               static_cast<int>(x) + size, static_cast<int>(y) + size, color);
      break;
    default:
      break;
  }
}
