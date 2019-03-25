#ifndef WORLD_H
#define WORLD_H

#include <allegro.h>
#include <png.h>
#include <loadpng.h>

#include <time.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "vec2.h"
#include "tools.h"

#include "globals.h"

#include "Entity.h"
#include "particle.h"

class world{
  public:
    world();
    virtual ~world();

    void update();
    void draw(BITMAP *buffer);

    void addParticle(particle *newParticle);

    void generate_map(int width, int height);
    void setup_tanks();
    void init_map(int width, int height);
    void place_barrier(int x, int y, int type);

    void AddEntity(Entity *entity);
    void RemoveEntity(Entity *entity);

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

    // Objects
    std::vector<Entity*> entities;
    std::vector<particle*> particles;
    std::vector<vec2<int>> startLocations;

    int map_temp[max_map_width][max_map_height];
    int map_x, map_y;

    int currentRound = 0;
};

#endif // WORLD_H
