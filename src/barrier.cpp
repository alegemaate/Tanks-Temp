#include "Barrier.h"

#include "World.h"
#include "Bullet.h"
#include "Powerup.h"

BITMAP* Barrier::images[3] = { nullptr };
SAMPLE* Barrier::sample_explode = nullptr;

Barrier::Barrier(World *wrld, float x, float y, int type) :
  Entity(wrld, x, y) {

  this -> indestructable = false;
  this -> image = nullptr;

  SetType(type);

  if (!sample_explode)
    sample_explode = load_sample_ex("sfx/explode.wav");
}

Barrier::~Barrier(){

}

void Barrier::SetType(int type) {
  // Ensure images are loaded
  if (images[0] == nullptr) {
    images[0] = load_bitmap_ex("images/block_stone_1.png");
    images[1] = load_bitmap_ex("images/block_box_1.png");
  }

  // Set type
  this -> type = type;

  // Select type
  switch (type) {
    case BARRIER_INDESTRUCTABLE:
    {
      image = images[0];
      SetHealth(0);
      SetIndestructable(true);
      break;
    }
    case BARRIER_STONE:
    {
      image = images[0];
      SetHealth(10);
      break;
    }
    case BARRIER_CRATE:
    {
      image = images[1];
      SetHealth(3);
      break;
    }
  }

  if (image)
    SetDimensions(vec2<int>(image -> w, image -> h));
}

void Barrier::SetHealth(int health){
  this -> health = health;
}

void Barrier::SetIndestructable(bool indestructable){
  this -> indestructable = indestructable;
}

// Update
void Barrier::Update() {
  if (health <= 0 && !indestructable) {
    // Explode
    Destroy();

    // Remove
    wrld -> RemoveEntity(this);
  }
}

// Collide
void Barrier::Collide(Entity *other) {
  // Bullet collide
  if (Bullet* pBullet = dynamic_cast<Bullet*>(other)) {
    if (!indestructable)
      health--;
  }
}

// Draw image
void Barrier::Draw(BITMAP* buffer) {
  if (image)
    draw_sprite(buffer, image, GetX(), GetY());
}

// Destroy
void Barrier::Destroy() {
  // Drop powerup
  if (type == BARRIER_CRATE) {
    wrld -> AddEntity(new Powerup(wrld, GetX(), GetY(), random(0, 3)));
  }

  // Explode
  play_sample(sample_explode, 255, 127, 1000, 0);

  for(int i = 0; i < 100; i++) {
    // position of colour
    int random_y = random(0, GetHeight());
    int random_x = random(0, GetWidth());

    // New colour
    int new_colour = getpixel(image, random_y, random_x);

    // Make particle
    if (!(getr(new_colour) == 255 && getg(new_colour) == 255 && getb(new_colour) == 255))
      wrld -> AddParticle(new Particle(GetX(), GetY(), new_colour, -6, 6, -6, 6, 1, CIRCLE, 30, EXPLODE));
  }
}
