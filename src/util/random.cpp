#include "random.h"

#include <chrono>

// Init generator
std::mt19937 Random::rng = std::mt19937(std::random_device{}());

// Random Float
float Random::randomFloat(const float min, const float max) {
  std::uniform_real_distribution<float> float_dist{min, max};
  return float_dist(rng);
}

// Random Int
int Random::randomInt(const int min, const int max) {
  std::uniform_int_distribution<int> int_dist{min, max};
  return int_dist(rng);
}
