#include "../include/barrier.h"

barrier::barrier( int newX, int newY){
  this -> x = newX;
  this -> y = newY;
}

barrier::~barrier(){

}

// Update
void barrier::update(){

}

// Draw image
void barrier::draw( BITMAP* tempImage){
  rectfill( tempImage, this -> x, this -> y, this -> x + 50, this -> y + 50, makecol( 25, 25, 25));
}
