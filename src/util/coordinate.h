#ifndef SRC_UTIL_COORDINATE_H_
#define SRC_UTIL_COORDINATE_H_

// Coordinate system
class Coordinate {
 public:
  Coordinate(float x, float y) : x(x), y(y) {}
  float x = 0;
  float y = 0;
};

#endif  // SRC_UTIL_COORDINATE_H_
