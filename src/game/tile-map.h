#ifndef SRC_GAME_TILE_MAP_H_
#define SRC_GAME_TILE_MAP_H_

#include <string>
#include <vector>

#include "../util/coordinate.h"
#include "barrier.h"

struct TileMapNeighbours {
  int n;
  int ne;
  int e;
  int se;
  int s;
  int sw;
  int w;
  int nw;
};

class TileMap {
 public:
  void generateMap(unsigned char width, unsigned char height);

 private:
  TileMapNeighbours getNeighbours(const unsigned char x,
                                  const unsigned char y) const;

  unsigned char width;
  unsigned char height;

  std::vector<Barrier> barriers;
  std::vector<Coordinate> start_locations;

  std::vector<std::vector<int>> tile_map;
};

#endif  // SRC_GAME_TILE_MAP_H_
