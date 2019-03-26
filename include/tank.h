#ifndef TANK_H
#define TANK_H

#include <allegro.h>

#include "Particle.h"
#include "Bullet.h"
#include "Barrier.h"
#include "Powerup.h"
#include "Entity.h"
#include "Timer.h"

class World;

enum tank_types {
  TANK_PLAYER,
  TANK_FRIEND,
  TANK_ENEMY
};

const float RAD_TO_FIX = 40.5845104792;

class tank : public Entity {
  public:
    explicit tank(World *wrld, float x, float y, int type);
    virtual ~tank();

    void SetupTank(int type);

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;
    virtual void Collide(Entity* other) override;

    void DrawDecal();
    void PickupPowerup(int powerup_id);

    int GetCenterX(){ return GetX() + GetWidth()/2; }
    int GetCenterY(){ return GetY() + GetHeight()/2; }

    void SetMapDimensions(int width, int height){ map_dimensions = vec2<int>(width, height); }

    static unsigned char num_bullet_bounces;

  protected:

    // Update
    virtual void Drive(float rotation);
    virtual void Shoot(float rotation, float x, float y);
    virtual void Destroy(int velocity, int amount, int life);

    // Draw
    virtual void DrawBase(BITMAP* buffer);
    virtual void DrawTurret(BITMAP* buffer);
    virtual void DrawHealthBar(BITMAP* buffer, int x, int y, int width, int height, int border_width);

    int health;
    int max_health;
    int fire_speed;

    Timer fire_timer;
    double fire_delay;

    int team;

    vec2<int> map_dimensions;

    float rotation_body;
    float rotation_turret;
    float total_velocity, max_velocity;

    bool canMoveX;
    bool canMoveY;

    static BITMAP* images[8];

    BITMAP *image_base;
    BITMAP *image_hurt;
    BITMAP *image_top;
    BITMAP *image_treads;

    static SAMPLE *sample_shot;
};

class player_tank: public tank{
  public:
    player_tank(World *wrld, int x, int y, int type);
    virtual void Update() override;
  protected:
};

class ai_tank: public tank{
  public:
    ai_tank(World *wrld, int x, int y, int type);
    virtual void Update() override;
  private:
    float destination_x;
    float destination_y;

    void update_target();
};

#endif // TANK_H
