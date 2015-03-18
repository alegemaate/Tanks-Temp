#ifndef BULLET_H
#define BULLET_H

#include "tools.h"

class bullet
{
  public:
    bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, SAMPLE* newSound);
    ~bullet();

    bool getErase();
    void update();
    void draw( BITMAP* tempImage);
  protected:
  private:
    float x;
    float y;
    float vector_x;
    float vector_y;
    bool on_screen;
    bool owner;
    bool pendingErase;
    SAMPLE* shotSound;
};

#endif // BULLET_H
