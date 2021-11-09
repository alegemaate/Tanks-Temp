#include "barrier.h"

#include "../system/ImageRegistry.h"
#include "../util/tools.h"

Barrier::Barrier(World* world, Coordinate position, BarrierType type)
    : position(position), worldPointer(world) {
  switch (type) {
    case BarrierType::BOX:
      this->image = ImageRegistry::getImage("block-box");
      this->health = 3;
      break;
    default:
      this->image = ImageRegistry::getImage("block-stone");
      this->health = 20;
      break;
  }

  this->width = static_cast<float>(this->image->w);
  this->height = static_cast<float>(this->image->h);
  this->sample_explode = load_sample_ex("sfx/explode.wav");
}

Barrier::~Barrier() {
  destroy_sample(sample_explode);
}

// Update
void Barrier::update(const std::vector<Bullet*>* bullets) {
  if (health <= 0 && !exploded) {
    explode(position.x + width / 2.0f, position.y + height / 2.0f, 6, 100, 30);
    exploded = true;
  }

  if (health <= 0) {
    return;
  }

  for (auto* const& bullet : *bullets) {
    if (collisionAny(position.x, position.x + width, bullet->getX(),
                     bullet->getX() + 5, position.y, position.y + height,
                     bullet->getY(), bullet->getY() + 5)) {
      if (collisionBottom(bullet->getY() + bullet->getYVelocity(),
                          bullet->getY() + 5, position.y,
                          position.y + height)) {
        bullet->reverseDirection("y");
        bullet->bounce(BounceDirection::BOTTOM);
      }

      if (collisionTop(bullet->getY(),
                       bullet->getY() + 5 + bullet->getYVelocity(), position.y,
                       position.y + height)) {
        bullet->reverseDirection("y");
        bullet->bounce(BounceDirection::TOP);
      }

      if (collisionLeft(bullet->getX(),
                        bullet->getX() + 5 + bullet->getXVelocity(), position.x,
                        position.x + width)) {
        bullet->reverseDirection("x");
        bullet->bounce(BounceDirection::LEFT);
      }

      if (collisionRight(bullet->getX() + bullet->getXVelocity(),
                         bullet->getX() + 5, position.x, position.x + width)) {
        bullet->reverseDirection("x");
        bullet->bounce(BounceDirection::RIGHT);
      }

      if (!indestructable) {
        health -= 1;
      }
    }
  }
}

// Draw image
void Barrier::draw(BITMAP* buffer) {
  if (health > 0 && visible) {
    draw_sprite(buffer, image, position.x, position.y);
  }
}

// Get width
int Barrier::getWidth() const {
  return width;
}

// Get height
int Barrier::getHeight() const {
  return height;
}

// Check if needs cleanup
bool Barrier::getDead() const {
  return !indestructable && health <= 0;
}

Coordinate Barrier::getPosition() {
  return this->position;
}

// Explode
void Barrier::explode(float x, float y, int velocity, int amount, int life) {
  // Explode
  play_sample(sample_explode, 255, 127, 1000, 0);

  for (int i = 0; i < amount; i++) {
    int color = 0;

    // Make sure not transparent (they show as white)
    do {
      // position of colour
      int randomY = random(0, height);
      int randomX = random(0, width);

      // New colour
      color = getpixel(image, randomY, randomX);
    } while (getr(color) == 255 && getg(color) == 255 && getb(color) == 255);

    // Make particle
    auto* particle =
        new Particle(x, y, color, -velocity, velocity, -velocity, velocity, 1,
                     ParticleType::CIRCLE, life, ParticleBehaviour::EXPLODE);

    worldPointer->addParticle(particle);
  }
}
