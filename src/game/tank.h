#ifndef SRC_GAME_TANK_H_
#define SRC_GAME_TANK_H_

#include <allegro.h>
#include <vector>

#include "./barrier.h"
#include "./bullet.h"
#include "./particle.h"
#include "./powerup.h"
#include "./world.h"

class Tank {
 public:
  explicit Tank(world* worldPointer,
                int x,
                int y,
                int hurtTime,
                int health,
                int fireSpeed,
                int fireDelay,
                float speed,
                BITMAP* baseImage,
                BITMAP* turretImage,
                BITMAP* hurtImage,
                BITMAP* treadsImage);
  virtual ~Tank();

  virtual bool isDead();

  virtual std::vector<Bullet>* getBullets();

  virtual void update();
  virtual void draw(BITMAP* buffer);
  virtual void putDecal(BITMAP* buffer);

  virtual void giveHealth(int healthAmount);

  virtual int getX() { return x; }
  virtual int getY() { return y; }

  virtual int getCenterX() { return x + width / 2; }
  virtual int getCenterY() { return y + height / 2; }

  virtual int getHeight() { return width; }
  virtual int getWidth() { return height; }

  virtual void set_map_dimensions(int width, int height) {
    map_width = width;
    map_height = height;
  }

  virtual void checkCollision(std::vector<Bullet>* bullets);
  virtual void checkCollision(std::vector<Barrier>* barriers);
  virtual void checkCollision(std::vector<Powerup>* powerups);

  virtual void process_enemies(std::vector<Tank*>* otherTanks);

  virtual void get_powerup(int powerupId);

  static unsigned char num_bullet_bounces;

 protected:
  float x;
  float y;

  int hurt_timer;
  int health;
  int initialHealth;
  int fire_speed;
  int fire_delay_rate;

  float max_speed;
  float speed;

  BITMAP* image_base;
  BITMAP* image_hurt;
  BITMAP* image_top;
  BITMAP* image_treads;

  world* worldPointer;

  bool dead;

  float rotation_radians_body;
  float rotation_allegro_body;
  float rotation_radians_turret;
  float rotation_allegro_turret;

  int bullet_delay;

  int width, height;

  int map_width, map_height;

  float vector_x;
  float vector_y;

  bool canMoveX;
  bool canMoveY;

  std::vector<Bullet> bullets;
  std::vector<Tank*>* otherTanks;

  SAMPLE* sample_shot;

  // Update
  virtual void drive(float rotation);
  virtual void shoot(float rotation, float x, float y);
  virtual void update_bullets();
  virtual void update_timers();
  virtual void explode(int x, int y, int velocity, int amount, int life);

  // Draw
  virtual void drawBullets(BITMAP* buffer);
  virtual void drawTankBase(BITMAP* buffer);
  virtual void drawTankTurret(BITMAP* buffer);
  virtual void drawHealthBar(BITMAP* buffer,
                             int x,
                             int y,
                             int width,
                             int height,
                             int border);
};

class PlayerTank : public Tank {
 public:
  PlayerTank(world* world,
             int x,
             int y,
             int hurtTime,
             int health,
             int fireSpeed,
             int fireDelay,
             float speed,
             BITMAP* baseImage,
             BITMAP* turretImage,
             BITMAP* hurtImage,
             BITMAP* treadsImage);
  virtual void update();

 protected:
};

class AiTank : public Tank {
 public:
  AiTank(world* world,
         int x,
         int y,
         int hurtTime,
         int health,
         int fireSpeed,
         int fireDelay,
         float speed,
         BITMAP* baseImage,
         BITMAP* turretImage,
         BITMAP* hurtImage,
         BITMAP* treadsImage);
  virtual void update();

 private:
  float destination_x;
  float destination_y;

  void update_target();
};

#endif  // SRC_GAME_TANK_H_
