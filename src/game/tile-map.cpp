#include "tile-map.h"

// Update
void TileMap::update() {}

// Draw map to screen
void TileMap::draw(BITMAP* buffer, int x, int y) {}

// Generate a new map
void TileMap::generateMap(unsigned char width, unsigned char height) {
  // Assign new width and height
  this->width = width;
  this->height = height;

  // Make a temp map
  const unsigned char const_width = width;
  const unsigned char const_height = height;
  int map_temp[const_width][const_height];

  // Make a map
  for (unsigned char pass = 0; pass < 8; pass++) {
    for (unsigned char i = 0; i < this->width; i++) {
      for (unsigned char t = 0; t < this->height; t++) {
        // Pass 0 (Erase)
        if (pass == 0) {
          map_temp[i][t] = 0;
        }
        // Pass 1 (Edges)
        else if (pass == 1) {
          if (i == 0 || t == 0 || i == this->width - 1 ||
              t == this->height - 1) {
            map_temp[i][t] = 1;
          }
        }
        // Pass 2 (Well Placed blocks)
        else if (pass == 2) {
          if (map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
              map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
              map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
              map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
              random(0, 2) == 1) {
            map_temp[i][t] = 1;
          }
        }
        // Pass 3 (Filling)
        else if (pass == 3) {
          if ((map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1) ||
              (map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1)) {
            map_temp[i][t] = 1;
          }
        }
        // Pass 4 (Filling inaccessable areas)
        else if (pass == 4) {
          if (map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1 &&
              map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1) {
            map_temp[i][t] = 1;
          }
        }
        // Pass 5 (Boxes!)
        else if (pass == 5) {
          if (map_temp[i][t] == 0 && random(1, 20) == 1) {
            map_temp[i][t] = 2;
          }
        }
        // Pass 6 (Find start locations)
        else if (pass == 6) {
          if (map_temp[i][t] == 0) {
            startLocations.push_back(Coordinate(i * 40, t * 40));
          }
        }
        // Pass 7 (create barriers)
        else if (pass == 7) {
          if (map_temp[i][t] == 1 || map_temp[i][t] == 2 ||
              map_temp[i][t] == 3) {
            /*barrier barrier( i * 40, t * 40, blocks[map_temp[i][t]], 0);

            // Destroyable
            if( map_temp[i][t] == 2)
              barrier.setHealth(3);
            else if( i != 0 && t != 0 && i != this -> width - 1 && t != this ->
            height - 1) barrier.setHealth(20); else
              barrier.setIndestructable(true);

            barriers.push_back( barrier);*/
          }
        }
      }
    }
  }
}
