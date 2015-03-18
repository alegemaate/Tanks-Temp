#include "../include/bullet.h"

// Init
bullet::bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, SAMPLE* newSound){
  x = newX;
  y = newY;
  vector_x = -newSpeed*cos( newAngle);
  vector_y = -newSpeed*sin( newAngle);
  owner = newOwner;
  shotSound = newSound;
  play_sample( shotSound, 255, 122, random(800,1200), 0);
  pendingErase = false;
}

bullet::~bullet(){

}

bool bullet::getErase(){
  return pendingErase;
}

// Update bullets
void bullet::update(){
  x += vector_x;
  y += vector_y;
  /*if(collisionAny( player_x, player_x + 50, x, x+5, player_y, player_y + 50, y, y + 5) && !owner){
    player_hurt_timer = 3;
    pendingErase = true;
    player_health -= 5;
  }*/
  if( x > 800 || x < 0){
    vector_x = -vector_x;
  }
  if( y > 600 || y < 0){
    vector_y = -vector_y;
  }
}

// Draw image
void bullet::draw( BITMAP* tempImage){
  if(on_screen){
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
}
