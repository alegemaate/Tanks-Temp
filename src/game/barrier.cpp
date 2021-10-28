#include "barrier.h"

Barrier::Barrier(world* world, int x, int y, BITMAP* image, int health)
    : x(x),
      y(y),
      image(image),
      worldPointer(world),
      health(health),
      indestructable(false),
      exploded(false),
      visible(true) {
  this->width = this->image->w;
  this->height = this->image->h;

  sample_explode = load_sample_ex("sfx/explode.wav");
}

Barrier::~Barrier() {
  destroy_sample(sample_explode);
}

// Update
void Barrier::update(std::vector<Bullet>* bullets) {
  if (health > 0 || indestructable) {
    for (unsigned int i = 0; i < bullets->size(); i++) {
      if (collisionAny(x, x + width, bullets->at(i).getX(),
                       bullets->at(i).getX() + 5, y, y + height,
                       bullets->at(i).getY(), bullets->at(i).getY() + 5)) {
        if (collisionBottom(
                bullets->at(i).getY() + bullets->at(i).getYVelocity(),
                bullets->at(i).getY() + 5, y, y + height)) {
          bullets->at(i).reverseDirection("y");
          bullets->at(i).bounce(BOTTOM);
        }

        if (collisionTop(
                bullets->at(i).getY(),
                bullets->at(i).getY() + 5 + bullets->at(i).getYVelocity(), y,
                y + height)) {
          bullets->at(i).reverseDirection("y");
          bullets->at(i).bounce(TOP);
        }

        if (collisionLeft(
                bullets->at(i).getX(),
                bullets->at(i).getX() + 5 + bullets->at(i).getXVelocity(), x,
                x + width)) {
          bullets->at(i).reverseDirection("x");
          bullets->at(i).bounce(LEFT);
        }

        if (collisionRight(
                bullets->at(i).getX() + bullets->at(i).getXVelocity(),
                bullets->at(i).getX() + 5, x, x + width)) {
          bullets->at(i).reverseDirection("x");
          bullets->at(i).bounce(RIGHT);
        }

        if (!indestructable) {
          health -= 1;
        }
      }
    }
  } else if (!exploded) {
    explode(x + width / 2, y + height / 2, 6, 100, 30);
    exploded = true;
  }
}

// Draw image
void Barrier::draw(BITMAP* buffer) {
  if ((health > 0 || indestructable) && visible) {
    draw_sprite(buffer, image, this->x, this->y);
  }
}

// Get x
int Barrier::getX() {
  return x;
}

// Get y
int Barrier::getY() {
  return y;
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
