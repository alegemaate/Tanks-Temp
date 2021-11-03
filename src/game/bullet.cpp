#include "./bullet.h"

#include <cmath>

// Init
Bullet::Bullet(World* world,
               float x,
               float y,
               float angle,
               float speed,
               int health,
               SAMPLE* sound) {
  this->worldPointer = world;
  this->x = x;
  this->y = y;
  this->vector_x = -speed * cos(angle);
  this->vector_y = -speed * sin(angle);
  this->shotSound = sound;
  play_sample(shotSound, 255, 127, random(800, 1200), 0);
  this->pendingErase = false;
  this->health = health;
}

Bullet::~Bullet() {}

bool Bullet::getErase() {
  return pendingErase;
}

// Reverse specified vector
void Bullet::reverseDirection(std::string direction) {
  if (direction == "x")
    vector_x = -vector_x;
  else if (direction == "y")
    vector_y = -vector_y;
  else {
    vector_y = -vector_y;
    vector_x = -vector_x;
  }
}

// Get coordinates
float Bullet::getX() {
  return x;
}
float Bullet::getY() {
  return y;
}

// Bounce off wall
void Bullet::bounce(int direction) {
  health--;
  incidenceDirection = direction;

  if (health <= 0)
    destroy();
}

// Destroy
void Bullet::destroy() {
  // Has it already died?
  if (!pendingErase) {
    // Make sure health is 0
    health = 0;

    // Make explosion
    for (int i = 0; i < 100; i++) {
      Particle* particle;
      int color = makecol(255, random(0, 255), 0);

      switch (incidenceDirection) {
        case (BOTTOM):
          particle =
              new Particle(x, y, color, -5, 5, 0, 3, 1, CIRCLE, 10, EXPLODE);
          break;
        case (TOP):
          particle =
              new Particle(x, y, color, -5, 5, -3, 0, 1, CIRCLE, 10, EXPLODE);
          break;
        case (LEFT):
          particle =
              new Particle(x, y, color, -3, 0, -5, 5, 1, CIRCLE, 10, EXPLODE);
          break;
        default:
          particle =
              new Particle(x, y, color, 0, 3, -5, 5, 1, CIRCLE, 10, EXPLODE);
          break;
      }

      worldPointer->addParticle(particle);
    }
  }

  // Die
  pendingErase = true;
}

// Return velocities
float Bullet::getXVelocity() {
  return vector_x;
}

float Bullet::getYVelocity() {
  return vector_y;
}

// Update bullets
void Bullet::update() {
  if (health > 0) {
    // Move
    x += vector_x;
    y += vector_y;

    // Off screen
    if (x < 0 || x > 10000 || y < 0 || y > 10000) {
      destroy();
    }
  }
}

// Draw image
void Bullet::draw(BITMAP* buffer) {
  if (health > 0) {
    rectfill(buffer, x, y, x + 5, y + 5, makecol(0, 0, 0));
    rectfill(buffer, x + 1, y + 1, x + 4, y + 4, makecol(255, 0, 0));
    rectfill(buffer, x + 2, y + 2, x + 3, y + 3, makecol(0, 255, 0));
  }
}
