#include "Barrier.hpp"

#include "../system/ImageRegistry.hpp"
#include "../util/Random.hpp"

Barrier::Barrier(World* world, const Vec2<float>& position, BarrierType type)
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
    explode();
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
                          bullet->getY() + 5, position.y + height)) {
        bullet->reverseDirection("y");
        bullet->bounce(BounceDirection::BOTTOM);
      }

      if (collisionTop(bullet->getY(),
                       bullet->getY() + 5 + bullet->getYVelocity(),
                       position.y)) {
        bullet->reverseDirection("y");
        bullet->bounce(BounceDirection::TOP);
      }

      if (collisionLeft(bullet->getX(),
                        bullet->getX() + 5 + bullet->getXVelocity(),
                        position.x)) {
        bullet->reverseDirection("x");
        bullet->bounce(BounceDirection::LEFT);
      }

      if (collisionRight(bullet->getX() + bullet->getXVelocity(),
                         bullet->getX() + 5, position.x + width)) {
        bullet->reverseDirection("x");
        bullet->bounce(BounceDirection::RIGHT);
      }

      if (!indestructible) {
        health -= 1;
      }
    }
  }
}

// Draw image
void Barrier::draw(BITMAP* buffer) {
  if (health > 0) {
    draw_sprite(buffer, image, static_cast<int>(position.x),
                static_cast<int>(position.y));
  }
}

// Get width
float Barrier::getWidth() const {
  return width;
}

// Get height
float Barrier::getHeight() const {
  return height;
}

// Check if it needs cleanup
bool Barrier::getDead() const {
  return !indestructible && health <= 0;
}

Vec2<float> Barrier::getPosition() const {
  return this->position;
}

// Explode
void Barrier::explode() {
  // Explode
  play_sample(sample_explode, 255, 127, 1000, 0);

  for (int i = 0; i < 100; i++) {
    int color;

    // Make sure not transparent (they show as white)
    do {
      // position of colour
      int randomY = Random::random(0, static_cast<int>(height));
      int randomX = Random::random(0, static_cast<int>(width));

      // New colour
      color = getpixel(image, randomY, randomX);
    } while (getr(color) == 255 && getg(color) == 255 && getb(color) == 255);

    // Make particle
    auto* particle = new Particle(
        position.x + width / 2.0f, position.y + height / 2.0f, color, -6.0, 6.0,
        -6.0, 6.0, 1, ParticleType::CIRCLE, 30, ParticleBehaviour::EXPLODE);

    worldPointer->addParticle(particle);
  }
}
