/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
**/
#ifndef GAME_H
#define GAME_H

#include <allegro.h>

#include "state.h"

class game : public state {
  public:
    game();
    ~game();

    void update();
    void draw();

  private:
    // Images
    BITMAP *buffer;
    BITMAP *cursor;
};

#endif // GAME_H
