#ifndef SRC_UTIL_TOOLS_H_
#define SRC_UTIL_TOOLS_H_

#include <asw/asw.h>
#include <string>

// Collision
extern bool collisionAny(float xMin1,
                         float xMax1,
                         float xMin2,
                         float xMax2,
                         float yMin1,
                         float yMax1,
                         float yMin2,
                         float yMax2);
extern bool collisionBottom(float yMin1, float yMax1, float yMax2);
extern bool collisionTop(float yMin1, float yMax1, float yMin2);
extern bool collisionRight(float xMin1, float xMax1, float xMax2);
extern bool collisionLeft(float xMin1, float xMax1, float xMin2);

// Finds angle of point 2 relative to point 1
extern float find_angle(float x_1, float y_1, float x_2, float y_2);
extern float find_distance(float x_1, float y_1, float x_2, float y_2);

extern float rad_to_deg(float radians);

#endif  // SRC_UTIL_TOOLS_H_
