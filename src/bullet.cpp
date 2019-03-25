#include "bullet.h"

// Init
bullet::bullet(world *wrld, float x, float y, float angle, float speed, int ownerID, int health, SAMPLE* sound) :
  Entity(wrld, x, y) {
  SetVelocity(vec2<float>(-speed*cos(angle), -speed*sin(angle)));

  this -> owner = ownerID;
  this -> shotSound = sound;
  play_sample(shotSound, 255, 127, random(800,1200), 0);
  this -> health = health;
}

bullet::~bullet(){

}

// Reverse specified vector
void bullet::ReverseDirection( std::string newDirection){
  if( newDirection == "x")
    velocity.x = -velocity.x;
  else if( newDirection == "y")
    velocity.y = -velocity.y;
  else{
    velocity.y = -velocity.y;
    velocity.x = -velocity.x;
  }
}

// Bounce off wall
void bullet::Bounce( int newDirection){
  health--;
  incidenceDirection = newDirection;

  if( health <= 0)
    Destroy();
}

// Destroy
void bullet::Destroy(){
  // Make sure health is 0
  health = 0;

  // Make explosion
  /*for( int i = 0; i < 100; i ++){
    particle *newParticle;
    if( incidenceDirection == BOTTOM){
      newParticle = new particle(GetX(), GetY(), makecol( 255, random(0,255), 0), -5, 5,  0, 3, 1, CIRCLE, 10, EXPLODE);
    }
    else if( incidenceDirection == TOP){
      newParticle = new particle(GetX(), GetY(), makecol( 255, random(0,255), 0), -5, 5, -3, 0, 1, CIRCLE, 10, EXPLODE);
    }
    else if( incidenceDirection == LEFT){
      newParticle = new particle(GetX(), GetY(), makecol( 255, random(0,255), 0), -3, 0, -5, 5, 1, CIRCLE, 10, EXPLODE);
    }
    else{
      newParticle = new particle(GetX(), GetY(), makecol( 255, random(0,255), 0),  0, 3, -5, 5, 1, CIRCLE, 10, EXPLODE);
    }
    //worldPointer -> addParticle(newParticle);
  }*/
}

// Update bullets
void bullet::Update() {
  if(health > 0){
    // Move
    position.x += velocity.x;
    position.y += velocity.y;

    // Off screen
    if (GetX() < 0 || GetX() > 10000 || GetY() < 0 || GetY() > 10000)
      Destroy();
  }
}

// Draw image
void bullet::Draw(BITMAP* buffer) {
  if( health > 0){
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
