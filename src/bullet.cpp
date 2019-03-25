#include "Bullet.h"

#include "world.h"

#include "Particle.h"
#include "Barrier.h"

// Init
Bullet::Bullet(world *wrld, float x, float y, float angle, float speed, int ownerID, int health, SAMPLE* sound) :
  Entity(wrld, x, y) {
  SetVelocity(vec2<float>(-speed*cos(angle), -speed*sin(angle)));

  this -> dimensions = vec2<int>(5, 5);
  this -> owner = ownerID;
  this -> shotSound = sound;
  play_sample(shotSound, 255, 127, random(800,1200), 0);
  this -> health = health;
}

Bullet::~Bullet(){

}

// Update bullets
void Bullet::Update() {
  // Move
  position.x += velocity.x;
  position.y += velocity.y;

  // Off screen
  if (health <= 0 || GetX() < 0 || GetX() > 10000 || GetY() < 0 || GetY() > 10000)
    Destroy();
}

// Collide
void Bullet::Collide(Entity *other) {
  // Barrier collide
  if (Barrier* pBarrier = dynamic_cast<Barrier*>(other)) {
    if (collisionBottom(GetY() + GetVelocity().y, GetY() + 5, pBarrier -> GetY(), pBarrier -> GetY() + pBarrier -> GetHeight())) {
      Bounce(BOTTOM);
    }
    if (collisionTop(GetY(), GetY() + 5 + GetVelocity().y, pBarrier -> GetY(), pBarrier -> GetY() + pBarrier -> GetHeight())) {
      Bounce(TOP);
    }
    if (collisionLeft(GetX(), GetX() + 5 + GetVelocity().x, pBarrier -> GetX(), pBarrier -> GetX() + pBarrier -> GetWidth())) {
      Bounce(LEFT);
    }
    if (collisionRight(GetX() + GetVelocity().x, GetX() + 5, pBarrier -> GetX(), pBarrier -> GetX() + pBarrier -> GetWidth())) {
      Bounce(RIGHT);
    }
  }
}

// Draw image
void Bullet::Draw(BITMAP* buffer) {
  if (health > 0){
    //if( owner == 0){
      rectfill(buffer, GetX()    , GetY()    , GetX() + 5, GetY() + 5, makecol(0,0,0));
      rectfill(buffer, GetX() + 1, GetY() + 1, GetX() + 4, GetY() + 4, makecol(255,0,0));
      rectfill(buffer, GetX() + 2, GetY() + 2, GetX() + 3, GetY() + 3, makecol(0,255,0));
    /*}
    else{
      rectfill( tempImage, x, y, x + 5, y + 5, makecol(255,0,0));
      rectfill( tempImage, x + 1, y + 1, x + 4, y + 4, makecol(255,0,0));
      rectfill( tempImage, x + 2, y + 2, x + 3, y + 3, makecol(255,0,0));
    }*/
  }
}

// Bounce off wall
void Bullet::Bounce(int direction) {
  health--;

  if (direction == LEFT || direction == RIGHT) {
    velocity.x = -velocity.x;
  }
  else if (direction == TOP || direction == BOTTOM) {
    velocity.y = -velocity.y;
  }
  else {
    velocity.y = -velocity.y;
    velocity.x = -velocity.x;
  }
}

// Destroy
void Bullet::Destroy(){
  // Make sure health is 0
  health = 0;

  // Make explosion
  for (int i = 0; i < 100; i++) {
    Particle *particle;
    if (lastBounceDirection == BOTTOM) {
      particle = new Particle(GetX(), GetY(), makecol(255, random(0,255), 0), -5, 5,  0, 3, 1, CIRCLE, 10, EXPLODE);
    }
    else if (lastBounceDirection == TOP) {
      particle = new Particle(GetX(), GetY(), makecol(255, random(0,255), 0), -5, 5, -3, 0, 1, CIRCLE, 10, EXPLODE);
    }
    else if (lastBounceDirection == LEFT) {
      particle = new Particle(GetX(), GetY(), makecol(255, random(0,255), 0), -3, 0, -5, 5, 1, CIRCLE, 10, EXPLODE);
    }
    else {
      particle = new Particle(GetX(), GetY(), makecol(255, random(0,255), 0),  0, 3, -5, 5, 1, CIRCLE, 10, EXPLODE);
    }
    wrld -> addParticle(particle);
  }

  wrld -> RemoveEntity(this);
}
