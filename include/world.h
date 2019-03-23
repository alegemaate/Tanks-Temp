#ifndef WORLD_H
#define WORLD_H

#include <allegro.h>
#include <png.h>
#include <loadpng.h>

#include <time.h>
#include <vector>

#include "coordinate.h"
#include "tools.h"

#include "barrier.h"
#include "tank.h"
#include "powerup.h"

#include "globals.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "particle.h"

class tank;
class bullet;
class powerup;

class world{
  public:
    world();
    virtual ~world();

    void update();
    void draw(BITMAP *buffer);

    void addParticle(particle *newParticle);

    // Play 3d sample
    void play_sample_2d(SAMPLE* spl, int vol, int pan, int freq, bool loop);

    void generate_map(int width, int height);
    void setup_tanks();
    void init_map(int width, int height);
    void place_barrier(int x, int y, int type);

    // Map stuff
    static const unsigned char max_map_width = 255;
    static const unsigned char max_map_height = 255;

    static unsigned char map_width;
    static unsigned char map_height;

    static unsigned char num_enemies;
    static unsigned char num_friends;

  protected:

  private:
    friend class ChatServer;

    // Images
    BITMAP *buffer;
    BITMAP *map_buffer;
    BITMAP *decal_buffer;
    BITMAP *background;
    BITMAP *blocks[3];
    BITMAP *powerup_images[4];
    BITMAP *tank_images[10];

    // Objects
    std::vector<Barrier*> barriers;
    std::vector<tank*> enemy_tanks;
    std::vector<tank*> player_tanks;
    std::vector<powerup> powerups;
    std::vector<coordinate> startLocations;
    std::vector<particle*> particles;

    int map_temp[max_map_width][max_map_height];
    int map_x, map_y;

    int currentRound = 0;
};

#endif // WORLD_H
