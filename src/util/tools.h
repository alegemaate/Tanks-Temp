#ifndef SRC_UTIL_TOOLS_H_
#define SRC_UTIL_TOOLS_H_

#include <allegro.h>
#include <string>

// Collision
extern bool collisionAny(int xMin1,
                         int xMax1,
                         int xMin2,
                         int xMax2,
                         int yMin1,
                         int yMax1,
                         int yMin2,
                         int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2);
extern bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2);

// Finds angle of point 2 relative to point 1
extern float find_angle(float x_1, float y_1, float x_2, float y_2);
extern float find_distance(float x_1, float y_1, float x_2, float y_2);

// Mouse position including resolution difference
extern int mouseX();
extern int mouseY();

// Random number generator
extern int random(int min, int max);
extern float randomf(float min, float max);

// Error reporting
extern void abort_on_error(const std::string& message);

// Load and error check images
extern BITMAP* load_bitmap_ex(const std::string& path);

// Load and error check sounds
extern SAMPLE* load_sample_ex(const std::string& path);

// Load and error check fonts
extern FONT* load_font_ex(const std::string& path);

int distanceTo2D(int x1, int y1, int x2, int y2);

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
                           float* i_y);

#endif  // SRC_UTIL_TOOLS_H_
