#ifndef TANK_H
#define TANK_H

#include <allegro.h>
#include "../include/particle.h"
#include "../include/bullet.h"
#include "../include/barrier.h"

class tank{
  public:
    explicit tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);

    bool getErase();

    vector<bullet>* getBullets();

    void update();
    void draw( BITMAP* tempImage);
    void putDecal( BITMAP* tempImage);

    int getX(){ return x; }
    int getY(){ return y; }

    int getHeight(){ return width; }
    int getWidth(){ return height; }

    void checkCollision( vector<bullet>* newBullets);
    void checkCollision( vector<barrier>* newBarriers);
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

    BITMAP *image_base;
    BITMAP *image_hurt;
    BITMAP *image_top;
    BITMAP *image_treads;

    SAMPLE *sample_shot;

    // Update
    void drive( float newRotation);
    void shoot( float newRotation, float newX, float newY);
    void update_bullets();
    void update_timers();
    void explode( int newX, int newY, int newVelocity, int newAmount, int newLife);
    bool isDead();

    // Draw
    void drawBullets( BITMAP* tempImage);
    void drawTankBase( BITMAP* tempImage);
    void drawTankTurret( BITMAP* tempImage);
    void drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight);
};

class player_tank: public tank{
  public:
    player_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    void update();
  protected:
};

class ai_tank: public tank{
  public:
    ai_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    void update();
    void process_enemies( vector<player_tank>* tempOtherTanks);
  protected:
    float destination_x;
    float destination_y;

    vector<player_tank>* otherTanks;
    void update_target();
};

#endif // TANK_H
