#include "tile-map.h"

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
        switch (pass) {
          // Pass 0 (Erase)
          case 0:
            map_temp[i][t] = 0;
            break;

          // Pass 1 (Edges)
          case 1:
            if (i == 0 || t == 0 || i == this->width - 1 ||
                t == this->height - 1) {
              map_temp[i][t] = 1;
            }
            break;

          // Pass 2 (Well Placed blocks)
          case 2:
            if (map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
                map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
                map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
                map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
                random(0, 2) == 1) {
              map_temp[i][t] = 1;
            }
            break;

          // Pass 3 (Filling)
          case 3:
            if ((map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1) ||
                (map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1)) {
              map_temp[i][t] = 1;
            }
            break;

          // Pass 4 (Filling inaccessable areas)
          case 4:
            if (map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1 &&
                map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1) {
              map_temp[i][t] = 1;
            }
            break;

          // Pass 5 (Boxes!)
          case 5:
            if (map_temp[i][t] == 0 && random(1, 20) == 1) {
              map_temp[i][t] = 2;
            }
            break;

          // Pass 6 (Find start locations)
          case 6:
            if (map_temp[i][t] == 0) {
              startLocations.push_back(Coordinate(i * 40, t * 40));
            }
            break;
        }
      }
    }
  }
