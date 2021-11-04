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
  explicit Tank(World* worldPointer,
                float x,
                float y,
                int health,
                int fireSpeed,
                int fireDelay,
                float speed);

  virtual ~Tank();

  virtual bool isDead();

  virtual std::vector<Bullet*>* getBullets();

  virtual void update();
  virtual void draw(BITMAP* buffer);
  virtual void putDecal(BITMAP* buffer);

  virtual float getX() { return x; }
  virtual float getY() { return y; }

  virtual float getCenterX() { return x + width / 2.0f; }
  virtual float getCenterY() { return y + height / 2.0f; }

  virtual int getHeight() { return width; }
  virtual int getWidth() { return height; }

  virtual void set_map_dimensions(int width, int height) {
    map_width = width;
    map_height = height;
  }

  virtual void checkCollision(std::vector<Bullet*>* bullets);
  virtual void checkCollision(std::vector<Barrier*>* barriers);
  virtual void checkCollision(std::vector<Powerup*>* powerups);

  virtual void process_enemies(std::vector<Tank*>* otherTanks);

  virtual void pickupPowerup(PowerupType type);

  static unsigned char num_bullet_bounces;

 protected:
  float x;
  float y;

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

  World* worldPointer;

  bool dead;

  float rotation_body;
  float rotation_turret;

  int bullet_delay;

  int width, height;

  int map_width, map_height;

  float vector_x;
  float vector_y;

  bool canMoveX;
  bool canMoveY;

  std::vector<Tank*>* otherTanks;

  // Update
  void drive(float rotation);
  void shoot(float rotation, float x, float y);
  void accelerate(bool moving);

 private:
  std::vector<Bullet*> bullets;

  SAMPLE* sample_shot;

  // Update
  void update_bullets();
  void explode(int x, int y, int velocity, int amount, int life);

  // Draw
  void drawBullets(BITMAP* buffer);
  void drawTankBase(BITMAP* buffer);
  void drawTankTurret(BITMAP* buffer);
  void drawHealthBar(BITMAP* buffer,
                     int x,
                     int y,
                     int width,
                     int height,
                     int border);
};

#endif  // SRC_GAME_TANK_H_
