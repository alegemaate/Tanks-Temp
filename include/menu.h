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

class menu : public state
{
  public:
    menu();
    ~menu(){};

    void update();
    void draw();

  protected:

  private:
    char num_enemies = 0;
    char num_friends = 0;

    button enemies_up;
    button enemies_down;
    button friends_up;
    button friends_down;
    button start;

    BITMAP *buffer;
    BITMAP *background;
};

#endif // MENU_H
