#include "barrier.h"

#include "../system/ImageRegistry.h"
#include "../util/tools.h"

Barrier::Barrier(World* world, Coordinate position, BarrierType type)
    : position(position),
      worldPointer(world),
      type(type),
      indestructable(false),
      exploded(false),
      visible(true) {
  switch (type) {
    case BarrierType::BOX:
      this->image = ImageRegistry::getImage("block-box");
      this->health = 3;
      break;
    case BarrierType::STONE:
    default:
      this->image = ImageRegistry::getImage("block-stone");
      this->health = 20;
      break;
  }

  this->width = this->image->w;
  this->height = this->image->h;
  this->sample_explode = load_sample_ex("sfx/explode.wav");
}

Barrier::~Barrier() {
  destroy_sample(sample_explode);
}

// Update
void Barrier::update(std::vector<Bullet*>* bullets) {
  if (health > 0 || indestructable) {
    for (auto const& bullet : *bullets) {
      if (collisionAny(position.x, position.x + width, bullet->getX(),
                       bullet->getX() + 5, position.y, position.y + height,
                       bullet->getY(), bullet->getY() + 5)) {
        if (collisionBottom(bullet->getY() + bullet->getYVelocity(),
                            bullet->getY() + 5, position.y,
                            position.y + height)) {
          bullet->reverseDirection("y");
          bullet->bounce(BOTTOM);
        }

        if (collisionTop(bullet->getY(),
                         bullet->getY() + 5 + bullet->getYVelocity(),
                         position.y, position.y + height)) {
          bullet->reverseDirection("y");
          bullet->bounce(TOP);
        }

        if (collisionLeft(bullet->getX(),
                          bullet->getX() + 5 + bullet->getXVelocity(),
                          position.x, position.x + width)) {
          bullet->reverseDirection("x");
          bullet->bounce(LEFT);
        }

        if (collisionRight(bullet->getX() + bullet->getXVelocity(),
                           bullet->getX() + 5, position.x,
                           position.x + width)) {
          bullet->reverseDirection("x");
          bullet->bounce(RIGHT);
        }

        if (!indestructable) {
          health -= 1;
        }
      }
    }
  } else if (!exploded) {
    explode(position.x + width / 2, position.y + height / 2, 6, 100, 30);
    exploded = true;
  }
}

// Draw image
void Barrier::draw(BITMAP* buffer) {
  if ((health > 0 || indestructable) && visible) {
    draw_sprite(buffer, image, position.x, position.y);
  }
}

// Get width
int Barrier::getWidth() {
  return width;
}

// Get height
int Barrier::getHeight() {
  return height;
}

// Check if needs cleanup
bool Barrier::getDead() {
  return !indestructable && health <= 0;
}

// Explode
void Barrier::explode(int x, int y, int velocity, int amount, int life) {
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
    Particle* particle =
        new Particle(x, y, color, -velocity, velocity, -velocity, velocity, 1,
                     CIRCLE, life, EXPLODE);

    worldPointer->addParticle(particle);
  }
}
