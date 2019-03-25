#include "Particle.h"

// Default Constructor
Particle::Particle() :
  Particle(0, 0, makecol(255,255,255), 0, 0, 0, 0, 1, 0, 0, 0) {
}

// Constructor
Particle::Particle(int x, int y, int colour, float x_min, float x_max, float y_min, float y_max, int size, int type, int life, int behaviour) {
  this -> x = x;
  this -> y = y;

  this -> size = size;
  this -> type = type;
  this -> life = life;
  this -> behaviour = behaviour;

  x_velocity = randomf(x_min, x_max);
  y_velocity = randomf(y_min, y_max);

  // No unmoving
  if (x_velocity == 0)
    x_velocity = 0.1;
  if (y_velocity == 0)
    y_velocity = 0.1;

  dead = false;
}

// Deconstructor
Particle::~Particle(){

}

// Set colour and extract values
void Particle::SetColour(int colour) {
  this -> colour = colour;
  this -> red = getr(colour);
  this -> green = getg(colour);
  this -> blue = getb(colour);
}

// Logic
void Particle::Update() {
  // Behaviour
  if (behaviour == EXPLODE) {
    x += x_velocity;
    y += y_velocity;
    x_velocity -= x_velocity/10;
    y_velocity -= y_velocity/10;
  }
  else {
    x += randomf(-x_velocity, x_velocity);
    y += randomf(-y_velocity, y_velocity);
  }

  // Die
  if (random(0, life) == 0)
    dead = true;
}

// Check death
bool Particle::IsDead() {
  return dead;
}

// Draw
void Particle::Draw(BITMAP* buffer) {
  if (type == PIXEL){
    putpixel(buffer, x, y, makecol(red, green, blue));
  }
  else if (type == SQUARE) {
    rectfill(buffer, x, y, x + size, y + size, makecol(red, green, blue));
  }
  else if (type == CIRCLE) {
    circlefill(buffer, x, y, size, makecol(red, green, blue));
  }
  else if (type == RANDOM) {
    switch(random(0,3)){
      case 0:
        putpixel(buffer, x, y, makecol(red, green, blue));
        break;
      case 1:
        circlefill(buffer, x, y, size, makecol(red, green, blue));
        break;
      case 2:
        rectfill(buffer, x, y, x + size, y + size, makecol(red, green, blue));
        break;
    }
  }
}
