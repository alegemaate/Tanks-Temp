#include "../include/barrier.h"

barrier::barrier( int newX, int newY, BITMAP* newImage){
  this -> x = newX;
  this -> y = newY;
  image = newImage;

  this -> width = image -> w;
  this -> height = image -> h;
}

barrier::~barrier(){

}

// Update
void barrier::update( vector<bullet>* newBullets){
  for( unsigned int i = 0; i < newBullets -> size(); i++){
    if( collisionAny( x, x + width, newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5, y, y + height, newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5)){
      if( collisionBottom( newBullets -> at(i).getY() + newBullets -> at(i).getYVelocity(), newBullets -> at(i).getY() + 5, y, y + height)){
        newBullets -> at(i).reverseDirection("y");
        newBullets -> at(i).bounceCounter( BOTTOM);
      }
      if( collisionTop( newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5 + newBullets -> at(i).getYVelocity(), y, y + height)){
        newBullets -> at(i).reverseDirection("y");
        newBullets -> at(i).bounceCounter( TOP);
      }
      if( collisionLeft( newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5 + newBullets -> at(i).getXVelocity(), x, x + width)){
        newBullets -> at(i).reverseDirection("x");
        newBullets -> at(i).bounceCounter( LEFT);
      }
      if( collisionRight( newBullets -> at(i).getX() + newBullets -> at(i).getXVelocity(), newBullets -> at(i).getX() + 5, x, x + width)){
        newBullets -> at(i).reverseDirection("x");
        newBullets -> at(i).bounceCounter( RIGHT);
      }
    }
  }
}

// Draw image
void barrier::draw( BITMAP* tempImage){
  draw_sprite( tempImage, image, this -> x, this -> y);
}
