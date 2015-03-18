#include "../include/bullet.h"

bullet::bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, SAMPLE* newSound){
  x = newX;
  y = newY;
  vector_x = -newSpeed*cos( newAngle);
  vector_y = -newSpeed*sin( newAngle);
  owner = newOwner;
  shotSound = newSound;
  play_sample( shotSound, 255, 122, random(800,1200), 0);
}

bullet::~bullet(){

}
