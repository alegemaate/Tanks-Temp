/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
**/
#ifndef MENU_H
#define MENU_H

#include <allegro.h>
#include <png.h>
#include <loadpng.h>

#include "tools.h"
#include "state.h"
#include "game.h"
#include "button.h"
#include "globals.h"

class menu : public state {
  public:
    menu();
    ~menu(){};

    void update();
    void draw();

  protected:

  private:
    button enemies_up;
    button enemies_down;
    button friends_up;
    button friends_down;
    button width_up;
    button width_down;
    button height_up;
    button height_down;
    button bounce_up;
    button bounce_down;
    button start;

    button host;
    button join;

    BITMAP *buffer;
    BITMAP *background;
};

#endif // MENU_H
