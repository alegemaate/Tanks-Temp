#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "tools.h"
#include "bullet.h"

using namespace std;

class barrier{
  public:
    barrier( int newX, int newY, BITMAP* newImage);
    ~barrier();

    void update( vector<bullet>* newBullets);
    void draw( BITMAP* tempImage);

    int getX();
    int getY();

    int getWidth();
    int getHeight();

    bool getDead();
  private:
    int x;
    int y;

    int width;
    int height;

    int health;

    vector<particle> explosionEffect;

    void explode( int newX, int newY, int newVelocity, int newAmount, int newLife);

    BITMAP* image;
    SAMPLE *sample_explode;
};

#endif // BARRIER_H
