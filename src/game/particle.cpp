#include "particle.h"

// Constructor
Particle::Particle(float x,
                   float y,
                   int color,
                   float xVeloctyMin,
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
  this->xVelocity = randomf(xVeloctyMin, xVelocityMax);
  this->yVelocity = randomf(yVelocityMin, yVelocityMax);

  // No unmoving
  if (xVelocity == 0) {
    this->xVelocity = 0.1;
  }
  if (yVelocity == 0) {
    this->yVelocity = 0.1;
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
    x += randomf(-xVelocity, xVelocity);
    y += randomf(-yVelocity, yVelocity);
  }

  // Die
  if (random(0, life) == 0) {
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
      putpixel(buffer, x, y, color);
      break;
    case ParticleType::SQUARE:
      rectfill(buffer, x, y, x + size, y + size, color);
      break;
    case ParticleType::CIRCLE:
      circlefill(buffer, x, y, size, color);
      break;
    case ParticleType::RANDOM:
      drawRandom(buffer);
      break;
    default:
      break;
  }
}

void Particle::drawRandom(BITMAP* buffer) const {
  switch (random(0, 3)) {
    case 0:
      putpixel(buffer, x, y, color);
      break;
    case 1:
      circlefill(buffer, x, y, size, color);
      break;
    case 2:
      rectfill(buffer, x, y, x + size, y + size, color);
      break;
    default:
      break;
  }
}
