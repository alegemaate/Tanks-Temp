/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
**/
#ifndef MENU_H
#define MENU_H

#include <allegro.h>
#include <alpng.h>
#include "tools.h"

#include "state.h"

class menu : public state
{
  public:
    menu();
    ~menu(){};

    void update();
    void draw();

  protected:

  private:
    BITMAP *buffer;
    BITMAP *background;
};

#endif // MENU_H
