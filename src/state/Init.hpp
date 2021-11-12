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
  void update() override;
  void draw() override {
    // No draw method
  }
};

#endif  // SRC_STATE_INIT_H_
