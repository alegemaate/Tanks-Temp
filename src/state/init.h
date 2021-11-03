/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#ifndef SRC_STATE_INIT_H_
#define SRC_STATE_INIT_H_

#include <allegro.h>

#include "./state.h"

class Init : public State {
 public:
  Init();
  ~Init(){};

  void update(){};
  void draw() {}

 protected:
 private:
};

#endif  // SRC_STATE_INIT_H_
