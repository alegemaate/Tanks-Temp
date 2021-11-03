#include "tools.h"

#include <loadpng.h>
#include <cmath>

// Collision
bool collisionAny(int xMin1,
                  int xMax1,
                  int xMin2,
                  int xMax2,
                  int yMin1,
                  int yMax1,
                  int yMin2,
                  int yMax2) {
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1) {
    return true;
  }
  return false;
}

bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2) {
  if (yMax1 > yMax2 && yMin1 < yMax2) {
    return true;
  }
  return false;
}

bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2) {
  if (yMax1 > yMin2 && yMin1 < yMin2) {
    return true;
  }
  return false;
}

bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2) {
  if (xMax1 > xMin2 && xMin1 < xMin2) {
    return true;
  }
  return false;
}

bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2) {
  if (xMax1 > xMax2 && xMin1 < xMax2) {
    return true;
  }
  return false;
}

// Random number generator. Use int random(lowest,highest);
int random(int min, int max) {
  int lowest = min, highest = max;
  int range = (highest - lowest) + 1;
  return lowest + static_cast<int>(range * rand() / (RAND_MAX + 1.0));
}

// Random float
float randomf(float min, float max) {
  float lowest = min, highest = max;
  float range = (highest - lowest) + 1;
  return lowest + static_cast<float>(range * rand() / (RAND_MAX + 1.0));
}

// Finds angle of point 2 relative to point 1
float find_angle(float x_1, float y_1, float x_2, float y_2) {
  return atan2(y_1 - y_2, x_1 - x_2);
}

// Finds distance between 2 points
float find_distance(float x_1, float y_1, float x_2, float y_2) {
  return hypot(x_1 - x_2, y_1 - y_2);
}

// ERROR REPORTING
void abort_on_error(const std::string& message) {
  if (screen != NULL) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  }
  allegro_message("%s.\n %s\n", message.c_str(), allegro_error);
  exit(-1);
}

// Load bitmap with error checked_array_iterator
BITMAP* load_bitmap_ex(const std::string& path) {
  BITMAP* bitmap = load_png(path.c_str(), NULL);
  if (!bitmap) {
    abort_on_error("Cannot find image " + path +
                   "\nPlease check your files and try again");
  }
  return bitmap;
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

// Load and error check fonts
FONT* load_font_ex(const std::string& path) {
  FONT* font = load_font(path.c_str(), NULL, NULL);
  if (!font) {
    abort_on_error("Cannot find font " + path +
                   "\nPlease check your files and try again");
  }

  return extract_font_range(font, ' ', 'z');
}

// Returns distance 2D
int distanceTo2D(int x1, int y1, int x2, int y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x,
                           float p0_y,
                           float p1_x,
                           float p1_y,
                           float p2_x,
                           float p2_y,
                           float p3_x,
                           float p3_y,
                           float* i_x,
                           float* i_y) {
  float s1_x, s1_y, s2_x, s2_y;
  s1_x = p1_x - p0_x;
  s1_y = p1_y - p0_y;
  s2_x = p3_x - p2_x;
  s2_y = p3_y - p2_y;

  float s, t;
  s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) /
      (-s2_x * s1_y + s1_x * s2_y);
  t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /
      (-s2_x * s1_y + s1_x * s2_y);

  if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
    // Collision detected
    if (i_x != NULL)
      *i_x = p0_x + (t * s1_x);
    if (i_y != NULL)
      *i_y = p0_y + (t * s1_y);
    return 1;
  }

  return 0;  // No collision
}
