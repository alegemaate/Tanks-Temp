#include "../include/barrier.h"

barrier::barrier( int newX, int newY){
  this -> x = newX;
  this -> y = newY;
}

barrier::~barrier(){

}

// Update
void barrier::update( vector<bullet>* newBullets){
  for( unsigned int i = 0; i < newBullets -> size(); i++){
    if( collisionAny( x, x + 50, newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5, y, y + 50, newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5)){
      if( collisionBottom( newBullets -> at(i).getY() + newBullets -> at(i).getYVelocity(), newBullets -> at(i).getY() + 5, y, y + 50)
         || collisionTop( newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5 + newBullets -> at(i).getYVelocity(), y, y + 50)){
        newBullets -> at(i).reverseDirection("y");
      }
      if( collisionLeft( newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5 + newBullets -> at(i).getXVelocity(), x, x + 50)
         || collisionRight( newBullets -> at(i).getX() + newBullets -> at(i).getXVelocity(), newBullets -> at(i).getX() + 5, x, x + 50)){
        newBullets -> at(i).reverseDirection("x");
      }
      newBullets -> at(i).bounceCounter();
    }
  }
}

// Draw image
void barrier::draw( BITMAP* tempImage){
  rectfill( tempImage, this -> x, this -> y, this -> x + 50, this -> y + 50, makecol( 25, 25, 25));
}
