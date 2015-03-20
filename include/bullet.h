#ifndef BULLET_H
#define BULLET_H

#include "tools.h"
#include "particle.h"
#include <vector>

class bullet
{
  public:
    bullet( float newX, float newY, float newAngle, float newSpeed, bool newOwner, int newHealth, SAMPLE* newSound);
    ~bullet();

    bool getErase();
    void update();
    void draw( BITMAP* tempImage);

    float getX();
    float getY();

    float getYVelocity();
    float getXVelocity();

    void bounceCounter( int newDirection);
    void reverseDirection( string newDirection);
  protected:
  private:
    int health;
    int incidenceDirection;

    float x;
    float y;
    float vector_x;
    float vector_y;

    bool exploded;
    bool owner;
    bool pendingErase;

    vector<particle> explosionEffect;

    SAMPLE* shotSound;
};

#endif // BULLET_H
