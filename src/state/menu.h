/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_MENU_H_
#define SRC_STATE_MENU_H_

#include <allegro.h>

#include "../ui/button.h"
#include "../util/tools.h"
#include "./game.h"
#include "./state.h"

class menu : public state {
 public:
  menu();
  ~menu() {}

  void update();
  void draw();

 protected:
 private:
  Button enemies_up;
  Button enemies_down;
  Button friends_up;
  Button friends_down;
  Button width_up;
  Button width_down;
  Button height_up;
  Button height_down;
  Button bounce_up;
  Button bounce_down;
  Button start;

  BITMAP* buffer;
  BITMAP* background;
};

#endif  // SRC_STATE_MENU_H_
