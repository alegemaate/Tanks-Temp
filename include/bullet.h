#ifndef BULLET_H
#define BULLET_H

#include "tools.h"
#include "particle.h"
#include <vector>

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define TANK 4

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

    void bounce( int newDirection);
    void destroy();

    void reverseDirection( std::string newDirection);
  protected:
  private:
    int health;
    int incidenceDirection;

    bool exploded;

    float x;
    float y;
    float vector_x;
    float vector_y;

    bool owner;
    bool pendingErase;

    std::vector<particle> explosionEffect;

    SAMPLE* shotSound;
};

#endif // BULLET_H
