#include "tools.h"

#include <loadpng.h>
#include <cmath>

#include "Random.hpp"

// Collision
bool collisionAny(float xMin1,
                  float xMax1,
                  float xMin2,
                  float xMax2,
                  float yMin1,
                  float yMax1,
                  float yMin2,
                  float yMax2) {
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1) {
    return true;
  }
  return false;
}

bool collisionBottom(float yMin1, float yMax1, float yMin2, float yMax2) {
  if (yMax1 > yMax2 && yMin1 < yMax2) {
    return true;
  }
  return false;
}

bool collisionTop(float yMin1, float yMax1, float yMin2, float yMax2) {
  if (yMax1 > yMin2 && yMin1 < yMin2) {
    return true;
  }
  return false;
}

bool collisionLeft(float xMin1, float xMax1, float xMin2, float xMax2) {
  if (xMax1 > xMin2 && xMin1 < xMin2) {
    return true;
  }
  return false;
}

bool collisionRight(float xMin1, float xMax1, float xMin2, float xMax2) {
  if (xMax1 > xMax2 && xMin1 < xMax2) {
    return true;
  }
  return false;
}

// Random number generator
int random(int min, int max) {
  return Random::randomInt(min, max);
}

// Random float
float randomf(float min, float max) {
  return Random::randomFloat(min, max);
}

// Finds angle of point 2 relative to point 1
float find_angle(float x_1, float y_1, float x_2, float y_2) {
  return atan2f(y_1 - y_2, x_1 - x_2);
}

// Finds distance between 2 points
float find_distance(float x_1, float y_1, float x_2, float y_2) {
  return hypotf(x_1 - x_2, y_1 - y_2);
}

// ERROR REPORTING
__attribute__((noreturn)) void abort_on_error(const std::string& message) {
  if (screen != nullptr) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  }
  allegro_message("%s.\n %s\n", message.c_str(), allegro_error);
  exit(-1);
}

// Load and error check sounds
SAMPLE* load_sample_ex(const std::string& path) {
  SAMPLE* sample = load_sample(path.c_str());
  if (!sample) {
    abort_on_error("Cannot find sound " + path +
                   "\nPlease check your files and try again");
  }

  return sample;
}

int radToFix(float radians) {
  auto i_rad = static_cast<int>(radians * 40.5845104792);
  return itofix(i_rad);
}
