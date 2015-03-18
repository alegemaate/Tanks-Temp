#ifndef BULLET_H
#define BULLET_H

#include <math.h>

class bullet
{
  public:
    bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, SAMPLE* newSound);
    ~bullet();

    float x;
    float y;
    float vector_x;
    float vector_y;
    bool on_screen;
    bool owner;
    SAMPLE* shotSound;

  protected:
  private:
};

#endif // BULLET_H
