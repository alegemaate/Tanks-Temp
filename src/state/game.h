/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_GAME_H_
#define SRC_STATE_GAME_H_

#include <allegro.h>
#include <time.h>
#include <array>
#include <cmath>
#include <memory>
#include <vector>

#include "../game/ai-tank.h"
#include "../game/barrier.h"
#include "../game/player-tank.h"
#include "../game/powerup.h"
#include "../game/tank.h"
#include "../game/world.h"
#include "../util/coordinate.h"
#include "../util/tools.h"

#include "./state.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

class Game : public State {
 public:
  Game();
  ~Game() override;

  void update() override;
  void draw() override;

  // Map stuff
  static const unsigned char max_map_width = 255;
  static const unsigned char max_map_height = 255;

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
  std::vector<std::unique_ptr<Powerup>> powerups;
  std::vector<Coordinate> startLocations;

  std::array<std::array<BarrierType, max_map_height>, max_map_width> map_temp;

  float map_x;
  float map_y;

  int currentRound = 0;
};

#endif  // SRC_STATE_GAME_H_
