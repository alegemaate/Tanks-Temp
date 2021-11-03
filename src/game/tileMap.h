#ifndef SRC_GAME_TILEMAP_H_
#define SRC_GAME_TILEMAP_H_

#include <string>
#include <vector>

#include "../util/coordinate.h"
#include "./barrier.h"

class TileMap {
 public:
  TileMap() {}
  virtual ~TileMap() {}

  void update();
  void draw(BITMAP* buffer, int x, int y);

  void generateMap(unsigned char width, unsigned char height);

 private:
  unsigned char width;
  unsigned char height;

  std::vector<Barrier> barriers;
  std::vector<Coordinate> startLocations;

  int map_temp[255][255];
};

#endif  // SRC_GAME_TILEMAP_H_
