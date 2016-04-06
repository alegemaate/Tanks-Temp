#ifndef TANK_H
#define TANK_H

#include <allegro.h>
#include "../include/particle.h"
#include "../include/bullet.h"
#include "../include/barrier.h"

class tank{
  public:
    explicit tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual ~tank();

    virtual bool getErase();

    virtual vector<bullet>* getBullets();

    virtual void update();
    virtual void draw( BITMAP* tempImage);
    virtual void putDecal( BITMAP* tempImage);

    virtual int getX(){ return x; }
    virtual int getY(){ return y; }

    virtual int getHeight(){ return width; }
    virtual int getWidth(){ return height; }

    virtual void checkCollision( vector<bullet>* newBullets);
    virtual void checkCollision( vector<barrier>* newBarriers);

    virtual void process_enemies( vector<tank*> *tempOtherTanks);
  protected:
    float x;
    float y;

    int width, height;

    int hurt_timer;
    int health;
    int initialHealth;
    int fire_speed;
    int fire_delay_rate;
    int bullet_delay;

    float rotation_radians_body;
    float rotation_allegro_body;
    float rotation_radians_turret;
    float rotation_allegro_turret;
    float vector_x;
    float vector_y;
    float speed;

    bool dead;
    bool pendingErase;

    bool canMoveX;
    bool canMoveY;

    vector<bullet> bullets;
    vector<particle> explosionEffect;
    vector<tank*> *otherTanks;

    BITMAP *image_base;
    BITMAP *image_hurt;
    BITMAP *image_top;
    BITMAP *image_treads;

    SAMPLE *sample_shot;

    // Update
    virtual void drive( float newRotation);
    virtual void shoot( float newRotation, float newX, float newY);
    virtual void update_bullets();
    virtual void update_timers();
    virtual void explode( int newX, int newY, int newVelocity, int newAmount, int newLife);
    virtual bool isDead();

    // Draw
    virtual void drawBullets( BITMAP* tempImage);
    virtual void drawTankBase( BITMAP* tempImage);
    virtual void drawTankTurret( BITMAP* tempImage);
    virtual void drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight, int newBorderWidth);
};

class player_tank: public tank{
  public:
    player_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual void update();
  protected:
};

class ai_tank: public tank{
  public:
    ai_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual void update();
  private:
    float destination_x;
    float destination_y;

    void update_target();
};

#endif // TANK_H
