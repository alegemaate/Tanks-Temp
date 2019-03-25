#ifndef TANK_H
#define TANK_H

#include <allegro.h>

#include "particle.h"
#include "bullet.h"
#include "barrier.h"
#include "powerup.h"
#include "Entity.h"

class bullet;
class world;
class Barrier;

enum tank_types {
  TANK_PLAYER,
  TANK_FRIEND,
  TANK_ENEMY
};

class tank : public Entity {
  public:
    explicit tank(world *wrld, float x, float y, int type);
    virtual ~tank();

    virtual void SetupTank(int type);

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;

    virtual void putDecal(BITMAP* buffer);

    virtual int getCenterX(){ return GetX() + GetWidth()/2; }
    virtual int getCenterY(){ return GetY() + GetHeight()/2; }

    virtual void set_map_dimensions(int map_width, int map_height){ this -> map_width = map_width; this -> map_height = map_height;}

    static unsigned char num_bullet_bounces;
  protected:
    int hurt_timer;
    int health;
    int initialHealth;
    int fire_speed;
    int fire_delay_rate;
    int bullet_delay;

    int map_width, map_height;

    float rotation_radians_body;
    float rotation_allegro_body;
    float rotation_radians_turret;
    float rotation_allegro_turret;
    float vector_x;
    float vector_y;
    float speed, max_speed;

    bool canMoveX;
    bool canMoveY;

    std::vector<bullet> bullets;
    std::vector<tank*> *otherTanks;

    static BITMAP* images[8];

    BITMAP *image_base;
    BITMAP *image_hurt;
    BITMAP *image_top;
    BITMAP *image_treads;

    static SAMPLE *sample_shot;

    // Update
    virtual void drive(float rotation);
    virtual void shoot(float rotation, float x, float y);
    virtual void update_bullets();
    virtual void update_timers();
    virtual void explode(int x, int y, int newVelocity, int newAmount, int newLife);

    // Draw
    virtual void drawTankBase(BITMAP* tempImage);
    virtual void drawTankTurret(BITMAP* tempImage);
    virtual void drawHealthBar(BITMAP* tempImage, int x, int y, int newWidth, int newHeight, int newBorderWidth);
};

class player_tank: public tank{
  public:
    player_tank(world *wrld, int x, int y, int type);
    virtual void Update() override;
  protected:
};

class ai_tank: public tank{
  public:
    ai_tank(world *wrld, int x, int y, int type);
    virtual void Update() override;
  private:
    float destination_x;
    float destination_y;

    void update_target();
};

#endif // TANK_H
