#include "../include/bullet.h"

// Init
bullet::bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, int newHealth, SAMPLE* newSound){
  x = newX;
  y = newY;
  vector_x = -newSpeed*cos( newAngle);
  vector_y = -newSpeed*sin( newAngle);
  owner = newOwner;
  shotSound = newSound;
  play_sample( shotSound, 255, 122, random(800,1200), 0);
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
void bullet::reverseDirection( string newDirection){
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

// Add to the bounce count
void bullet::bounceCounter(){
  health--;
}

// Return vectors
float bullet::getXVelocity(){
  return vector_x;
}
float bullet::getYVelocity(){
  return vector_y;
}

// Update bullets
void bullet::update(){
  if(!exploded){
    x += vector_x;
    y += vector_y;
    /*if(collisionAny( player_x, player_x + 50, x, x+5, player_y, player_y + 50, y, y + 5) && !owner){
      player_hurt_timer = 3;
      pendingErase = true;
      player_health -= 5;
    }*/
    if( x > 800 || x < 0){
      vector_x = -vector_x;
      bounceCounter();
    }
    if( y > 600 || y < 0){
      vector_y = -vector_y;
      bounceCounter();
    }

    if( health <= 0){
      exploded = true;
      // Make explosion
      for( int i = 0; i < 20; i ++){
        particle newParticle(x, y, makecol(255,0,0), -20, 20, -20, 20, 1, SQUARE, 10);
        explosionEffect.push_back(newParticle);
      }
    }
  }
  if( exploded){
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
  if(!exploded){
    if(owner){
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
