#include "../include/bullet.h"

// Init
bullet::bullet( float newX, float newY, float newAngle, float newSpeed, int newOwnerID, int newHealth, SAMPLE* newSound){
  x = newX;
  y = newY;
  vector_x = -newSpeed*cos( newAngle);
  vector_y = -newSpeed*sin( newAngle);
  owner = newOwnerID;
  shotSound = newSound;
  play_sample( shotSound, 255, 127, random(800,1200), 0);
  pendingErase = false;
  health = newHealth;
  exploded = false;
}

bullet::~bullet(){

}

bool bullet::getErase(){
  return pendingErase;
}

// Reverse specified vector
void bullet::reverseDirection( std::string newDirection){
  if( newDirection == "x")
    vector_x = -vector_x;
  else if( newDirection == "y")
    vector_y = -vector_y;
  else{
    vector_y = -vector_y;
    vector_x = -vector_x;
  }
}

// Get coordinates
float bullet::getX(){
  return x;
}
float bullet::getY(){
  return y;
}

// Bounce off wall
void bullet::bounce( int newDirection){
  health--;
  incidenceDirection = newDirection;

  if( health <= 0)
    destroy();
}

// Destroy
void bullet::destroy(){
  // Make sure health is 0
  health = 0;

  // Make explosion
  if( !exploded){
    exploded = true;
    for( int i = 0; i < 100; i ++){
      if( incidenceDirection == BOTTOM){
        particle newParticle( x, y, makecol( 255, random(0,255), 0), -5, 5,  0, 3, 1, CIRCLE, 10, EXPLODE);
        explosionEffect.push_back(newParticle);
      }
      else if( incidenceDirection == TOP){
        particle newParticle( x, y, makecol( 255, random(0,255), 0), -5, 5, -3, 0, 1, CIRCLE, 10, EXPLODE);
        explosionEffect.push_back(newParticle);
      }
      else if( incidenceDirection == LEFT){
        particle newParticle( x, y, makecol( 255, random(0,255), 0), -3, 0, -5, 5, 1, CIRCLE, 10, EXPLODE);
        explosionEffect.push_back(newParticle);
      }
      else if( incidenceDirection == RIGHT){
        particle newParticle( x, y, makecol( 255, random(0,255), 0),  0, 3, -5, 5, 1, CIRCLE, 10, EXPLODE);
        explosionEffect.push_back(newParticle);
      }
    }
  }
}

// Return velocities
float bullet::getXVelocity(){
  return vector_x;
}
float bullet::getYVelocity(){
  return vector_y;
}

// Update bullets
void bullet::update(){
  if( health > 0){
    // Move
    x += vector_x;
    y += vector_y;

    // Off screen
    if( x < 0 || x > 10000 || y < 0 || y > 10000)
      destroy();
  }
  else{
    // Update particles
    for( unsigned int i = 0; i < explosionEffect.size(); i++){
      explosionEffect.at(i).logic();
      //Check death of particles
      if(explosionEffect.at(i).getDead())
        explosionEffect.erase(explosionEffect.begin() + i);
    }

    // Delete bullet once particles are all dead
    if( explosionEffect.size() <= 0)
      pendingErase = true;
  }
}

// Draw image
void bullet::draw( BITMAP* tempImage){
  if( health > 0){
    if( owner == 0){
      rectfill( tempImage, x, y, x + 5, y + 5, makecol(0,0,0));
      rectfill( tempImage, x + 1, y + 1, x + 4, y + 4, makecol(255,0,0));
      rectfill( tempImage, x + 2, y + 2, x + 3, y + 3, makecol(0,255,0));
    }
    else{
      rectfill( tempImage, x, y, x + 5, y + 5, makecol(255,0,0));
      rectfill( tempImage, x + 1, y + 1, x + 4, y + 4, makecol(255,0,0));
      rectfill( tempImage, x + 2, y + 2, x + 3, y + 3, makecol(255,0,0));
    }
  }
  else{
    for( unsigned int i = 0; i < explosionEffect.size(); i++){
      explosionEffect.at(i).draw(tempImage);
    }
  }
}
