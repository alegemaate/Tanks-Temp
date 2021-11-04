#include "particle.h"

// Default Constructor
Particle::Particle() {
  x = 0;
  y = 0;

  color = makecol(255, 255, 255);

  size = 1;
  type = 0;
  behaviour = 0;

  life = 0;

  xVelocity = 0;
  yVelocity = 0;

  dead = false;
}

// Constructor
Particle::Particle(float x,
                   float y,
                   int color,
                   float xVeloctyMin,
                   float xVelocityMax,
                   float yVelocityMin,
                   float yVelocityMax,
                   int size,
                   int type,
                   int life,
                   int behaviour)
    : x(x),
      y(y),
      color(color),
      size(size),
      type(type),
      behaviour(behaviour),
      life(life),
      dead(false) {
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
  if (behaviour == EXPLODE) {
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
bool Particle::getDead() {
  return dead;
}

// Draw
void Particle::draw(BITMAP* buffer) {
  switch (type) {
    case (PIXEL):
      putpixel(buffer, x, y, color);
      break;
    case (SQUARE):
      rectfill(buffer, x, y, x + size, y + size, color);
      break;
    case (CIRCLE):
      circlefill(buffer, x, y, size, color);
      break;
    case (RANDOM):
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
      }
  }
}
