#include "init.h"

// Init state (and game)
init::init(){
  // Goto game
  set_next_state(STATE_MENU);
}
