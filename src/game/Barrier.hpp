#ifndef SRC_GAME_BARRIER_H_
#define SRC_GAME_BARRIER_H_

#include <vector>

#include "../util/Vec2.hpp"
#include "./Bullet.hpp"

enum class BarrierType {
  NONE,
  BOX,
  STONE,
};

class Barrier {
 public:
  Barrier(World* world, const Vec2<float>& position, BarrierType type);

  void update(const std::vector<Bullet*>* bullets);
  void draw();

  float getWidth() const;
  float getHeight() const;

  void makeIndestructible(bool i) { this->indestructible = i; }

  bool getDead() const;

  Vec2<float> getPosition() const;

 private:
  Vec2<float> position;

  World* worldPointer;

  int health;

  bool indestructible = false;
  bool exploded = false;

  float width;
  float height;

  static asw::Sample sample_explode;

   asw::Texture image;

  void explode();
};

#endif  // SRC_GAME_BARRIER_H_
