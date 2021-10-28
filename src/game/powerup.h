#ifndef SRC_GAME_POWERUP_H_
#define SRC_GAME_POWERUP_H_

#include <allegro.h>

class Powerup {
 public:
  Powerup();
  Powerup(int x, int y, int type, BITMAP* image);
  virtual ~Powerup();

  int getX() { return x; }
  int getY() { return y; }
  int getWidth() { return image->w; }
  int getHeight() { return image->h; }
  int getType() { return type; }

  bool getDead() { return dead; }

  void pickup() { dead = true; }

  void draw(BITMAP* tempBitmap);

 private:
  BITMAP* image;

  int x;
  int y;
  int type;

  bool dead;
};

#endif  // SRC_GAME_POWERUP_H_
