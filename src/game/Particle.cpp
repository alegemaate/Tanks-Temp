#include "Particle.hpp"
#include <iostream>
#include "../util/Random.hpp"

// Constructor
Particle::Particle(float x,
                   float y,
                   asw::Color color,
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
void Particle::update(const float deltaTime) {
  float deltaXVelocity = xVelocity * (deltaTime / 8.0f);
  float deltaYVelocity = yVelocity * (deltaTime / 8.0f);

  // Behaviour
  if (behaviour == ParticleBehaviour::EXPLODE) {
    x += deltaXVelocity;
    y += deltaYVelocity;
    xVelocity -= deltaXVelocity / 10.0f;
    yVelocity -= deltaYVelocity / 10.0f;
  } else {
    x += Random::randomFloat(-deltaXVelocity, deltaXVelocity);
    y += Random::randomFloat(-deltaYVelocity, deltaYVelocity);
  }

  // Die
  if (Random::random(0, life) < (deltaTime / 8.0f)) {
    dead = true;
  }
}

// Check death
bool Particle::getDead() const {
  return dead;
}

// Draw
void Particle::draw() const {
  switch (type) {
    case ParticleType::PIXEL:
      asw::draw::point(static_cast<int>(x), static_cast<int>(y), color);
      break;
    case ParticleType::SQUARE:
      asw::draw::rectFill(static_cast<int>(x), static_cast<int>(y), size, size,
                          color);
      break;
    case ParticleType::CIRCLE:
      asw::draw::circleFill(static_cast<int>(x), static_cast<int>(y), size,
                            color);
      break;
    case ParticleType::RANDOM:
      drawRandom();
      break;
    default:
      break;
  }
}

void Particle::drawRandom() const {
  switch (Random::random(0, 3)) {
    case 0:
      asw::draw::point(static_cast<int>(x), static_cast<int>(y), color);
      break;
    case 1:
      asw::draw::circleFill(static_cast<int>(x), static_cast<int>(y), size,
                            color);
      break;
    case 2:
      asw::draw::rectFill(static_cast<int>(x), static_cast<int>(y), size, size,
                          color);
      break;
    default:
      break;
  }
}
