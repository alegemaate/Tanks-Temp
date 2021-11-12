#ifndef SRC_UTIL_RANDOM_H_
#define SRC_UTIL_RANDOM_H_

#include <random>

class Random {
 public:
  static float randomFloat(float min, float max);

  static int random(int min, int max);

 private:
  /// Random number generator
  static std::mt19937 rng;
};

#endif  // SRC_UTIL_RANDOM_H_