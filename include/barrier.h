#ifndef BARRIER_H
#define BARRIER_H

#include <allegro.h>

class barrier{
  public:
    barrier( int newX, int newY);
    ~barrier();

    void update();
    void draw( BITMAP* tempImage);
  private:
    int x;
    int y;
};

#endif // BARRIER_H
