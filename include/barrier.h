#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "tools.h"
#include "bullet.h"

using namespace std;

class barrier{
  public:
    barrier( int newX, int newY);
    ~barrier();

    void update( vector<bullet>* newBullets);
    void draw( BITMAP* tempImage);
  private:
    int x;
    int y;
};

#endif // BARRIER_H
