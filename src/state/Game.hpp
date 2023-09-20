/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_GAME_H_
#define SRC_STATE_GAME_H_

#include <allegro.h>
#include <array>
#include <memory>
#include <vector>

#include "../game/AiTank.hpp"
#include "../game/Barrier.hpp"
#include "../game/PlayerTank.hpp"
#include "../game/PowerUp.hpp"
#include "../game/Tank.hpp"
#include "../game/World.hpp"
#include "../util/Vec2.hpp"
#include "../util/tools.h"

#include "./State.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

class Game : public State {
 public:
  Game();
  ~Game() override;

  void update(const double deltaTime) override;
  void draw() override;

  // Map stuff
  static unsigned char map_width;
  static unsigned char map_height;

  static unsigned char num_enemies;
  static unsigned char num_friends;

 private:
  // Images
  BITMAP* buffer;
  BITMAP* map_buffer;
  BITMAP* decal_buffer;
  BITMAP* vision_buffer;
  BITMAP* background;
  BITMAP* cursor;

  // World
  World game_world;

  // Objects
  std::vector<std::unique_ptr<Barrier>> barriers;
  std::vector<Tank*> enemy_tanks;
  std::vector<Tank*> player_tanks;
  std::vector<std::unique_ptr<PowerUp>> power_ups;
  std::vector<Vec2<float>> startLocations;

  float map_x = 0;
  float map_y = 0;

  int currentRound = 0;
};

#endif  // SRC_STATE_GAME_H_
