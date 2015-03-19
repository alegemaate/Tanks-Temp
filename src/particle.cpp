#include "../include/particle.h"

// Constructor
particle::particle(int newX, int newY, int newColor, int newXMin, int newXMax, int newYMin, int newYMax, int newSize, int newType, int newLife){
  x = newX;
  y = newY;

  particleColor = newColor;
  particleRed = getr(particleColor);
  particleGreen = getg(particleColor);
  particleBlue = getb(particleColor);

  particleSize = newSize;

  xMin = newXMin;
  xMax = newXMax;
  yMin = newYMin;
  yMax = newYMax;

  particleType = newType;

  dead = false;
  particleLife = newLife;
}

// Deconstructor
particle::~particle(){

}

// Logic
void particle::logic(){
  x += random( xMin, xMax);
  y += random( yMin, yMax);

  if(x < 0){
    x = 0;
  }
  if(x > 800){
    x = 800;
  }
  if(y < 0){
    y = 0;
  }
  if(y > 600){
    y = 600;
  }

  // Die
  if( random( 0, particleLife) == 0)
    dead = true;
}

// Check death
bool particle::getDead(){
  return dead;
}

// Draw
void particle::draw( BITMAP* tempBitmap){
  if(particleType == PIXEL){
    putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == SQUARE){
    rectfill( tempBitmap, x, y, x + particleSize, y + particleSize, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == CIRCLE){
    circlefill( tempBitmap, x, y, particleSize, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == RANDOM){
    switch(random(0,3)){
      case 0:
        putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 1:
        circlefill( tempBitmap, x, y, particleSize, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 2:
        rectfill( tempBitmap, x, y, x + particleSize, y + particleSize, makecol( particleRed, particleGreen, particleBlue));
        break;
    }
  }
}
