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
#include "button.h"
#include "globals.h"

class menu : public state
{
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
    button start;

    BITMAP *buffer;
    BITMAP *background;
};

#endif // MENU_H
