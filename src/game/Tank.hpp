#ifndef SRC_GAME_TANK_H_
#define SRC_GAME_TANK_H_

#include <allegro.h>
#include <memory>
#include <vector>

#include "./Barrier.hpp"
#include "./Bullet.hpp"
#include "./Particle.hpp"
#include "./PowerUp.hpp"
#include "./World.hpp"

class Tank {
 public:
  explicit Tank(World* worldPointer,
                float x,
                float y,
                int health,
                int fireSpeed,
                int fireDelay,
                float speed);

  virtual ~Tank() = default;

  virtual bool isDead();

  virtual std::vector<Bullet*>* getBullets();

  virtual void update();
  virtual void draw(BITMAP* buffer);
  virtual void putDecal(BITMAP* buffer);

  virtual float getX() { return x; }
  virtual float getY() { return y; }

  virtual float getCenterX() { return x + width / 2.0f; }
  virtual float getCenterY() { return y + height / 2.0f; }

  virtual void set_map_dimensions(int mWidth, int mHeight) {
    map_width = mWidth;
    map_height = mHeight;
  }

  virtual void checkCollision(std::vector<Bullet*>* enemyBullets);
  virtual void checkCollision(
      const std::vector<std::unique_ptr<Barrier>>& barriers);
  virtual void checkCollision(
      const std::vector<std::unique_ptr<PowerUp>>& power_ups);

  virtual void process_enemies(std::vector<Tank*>* otherTanks);

  virtual void pickupPowerUp(PowerUpType type);

  static unsigned char num_bullet_bounces;

 protected:
  float x;
  float y;

  int health;
  int initialHealth;
  int fire_speed;
  int fire_delay_rate;

  float max_speed;
  float speed = 0;

  BITMAP* image_base;
  BITMAP* image_hurt;
  BITMAP* image_top;
  BITMAP* image_treads;

  World* worldPointer;

  bool dead = false;

  float rotation_body = 0;
  float rotation_turret = 0;

  int bullet_delay = 0;

  float width = 0;
  float height = 0;

  int map_width;
  int map_height;

  float vector_x = 0;
  float vector_y = 0;

  bool canMoveX = true;
  bool canMoveY = true;

  std::vector<Tank*>* otherTanks;

  // Update
  void drive(float rotation);
  void shoot(float rotation, float targetX, float targetY);
  void accelerate(bool moving);

 private:
  std::vector<Bullet*> bullets;

  SAMPLE* sample_shot;

  // Update
  void update_bullets();
  void explode();

  // Draw
  void drawBullets(BITMAP* buffer) const;
  void drawTankBase(BITMAP* buffer);
  void drawTankTurret(BITMAP* buffer);
  void drawHealthBar(BITMAP* buffer,
                     float x,
                     float y,
                     int width,
                     int height,
                     int border) const;
};

#endif  // SRC_GAME_TANK_H_
