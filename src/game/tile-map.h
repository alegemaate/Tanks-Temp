#ifndef SRC_GAME_TILE_MAP_H_
#define SRC_GAME_TILE_MAP_H_

#include <string>
#include <vector>

#include "../util/coordinate.h"
#include "barrier.h"

class TileMap {
 public:
  void generateMap(unsigned char width, unsigned char height);

 private:
  unsigned char width;
  unsigned char height;

  std::vector<Barrier> barriers;
  std::vector<Coordinate> startLocations;

  int map_temp[255][255];
};

#endif  // SRC_GAME_TILE_MAP_H_
