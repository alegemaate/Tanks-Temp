/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
**/
#ifndef GAME_H
#define GAME_H

#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <vector>

#include "barrier.h"
#include "tools.h"
#include "tank.h"
#include "powerup.h"

#include "state.h"

using namespace std;

// Coordinate system
struct coordinate{
  int x = 0;
  int y = 0;
};

class game : public state
{
  public:
    game();
    ~game(){};

    void update();
    void draw();

  private:
    // Images
    BITMAP *buffer;
    BITMAP *map_buffer;
    BITMAP *decal_buffer;
    BITMAP *vision_buffer;
    BITMAP *background;
    BITMAP *cursor;
    BITMAP *blocks[3];
    BITMAP *powerup_images[4];
    BITMAP *tank_images[10];

    // Objects
    vector<barrier> barriers;
    vector<tank*> enemy_tanks;
    vector<tank*> player_tanks;
    vector<powerup> powerups;
    vector<coordinate> startLocations;

    // Map stuff
    static const int map_width = 1600/40;
    static const int map_height = 1200/40;
    int map_temp[map_width][map_height];
    int map_x, map_y;

    int currentRound = 0;
};

#endif // GAME_H
