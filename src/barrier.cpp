#include "barrier.h"
#include "tools.h"

#include "world.h"
#include "powerup.h"

BITMAP* Barrier::images[3] = { nullptr };
SAMPLE* Barrier::sample_explode = nullptr;

Barrier::Barrier(world *wrld, float x, float y, int type) :
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
  if(health > 0 || indestructable) {
    /*for( unsigned int i = 0; i < newBullets -> size(); i++){
      if( collisionAny( x, x + width, newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5, y, y + height, newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5)){
        if( collisionBottom( newBullets -> at(i).getY() + newBullets -> at(i).getYVelocity(), newBullets -> at(i).getY() + 5, y, y + height)){
          newBullets -> at(i).reverseDirection("y");
          newBullets -> at(i).bounce( BOTTOM);
        }
        if( collisionTop( newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5 + newBullets -> at(i).getYVelocity(), y, y + height)){
          newBullets -> at(i).reverseDirection("y");
          newBullets -> at(i).bounce( TOP);
        }
        if( collisionLeft( newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5 + newBullets -> at(i).getXVelocity(), x, x + width)){
          newBullets -> at(i).reverseDirection("x");
          newBullets -> at(i).bounce( LEFT);
        }
        if( collisionRight( newBullets -> at(i).getX() + newBullets -> at(i).getXVelocity(), newBullets -> at(i).getX() + 5, x, x + width)){
          newBullets -> at(i).reverseDirection("x");
          newBullets -> at(i).bounce( RIGHT);
        }
        if( !indestructable)
          health -= 1;
      }
    }*/
  }
  if (health <= 0 && !indestructable) {
    // Drop powerup
    if (type == BARRIER_CRATE) {
      wrld -> AddEntity(new powerup(wrld, GetX(), GetY(), random(0, 4)));
    }

    // Explode
    Destroy(GetX() + GetWidth() / 2, GetY() + GetHeight() / 2, 6, 100, 30);

    // Remove
    wrld -> RemoveEntity(this);
  }
}

// Draw image
void Barrier::Draw(BITMAP* buffer) {
  if (image)
    draw_sprite(buffer, image, GetX(), GetY());
}

// Destroy
void Barrier::Destroy(int x, int y, int velocity, int amount, int life) {
  // Explode
  play_sample(sample_explode, 255, 127, 1000, 0);

  for(int i = 0; i < amount; i++) {
    int new_colour = 0;

    // Make sure not transparent ( they show as white)
    do {
      // position of colour
      int random_y = random(0, GetHeight());
      int random_x = random(0, GetWidth());

      // New colour
      new_colour = getpixel(image, random_y, random_x);
    } while(getr(new_colour) == 255 && getg(new_colour) == 255 && getb(new_colour) == 255);

    // Make particle
    particle *newParticle = new particle(x, y, new_colour, -velocity, velocity, -velocity, velocity, 1, CIRCLE, life, EXPLODE);
    wrld -> addParticle(newParticle);
  }
}
