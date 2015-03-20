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
  private:
    int x;
    int y;

    int width;
    int height;

    BITMAP* image;
};

#endif // BARRIER_H
