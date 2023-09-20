/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#ifndef TANKS_INIT_HPP
#define TANKS_INIT_HPP

#include <allegro.h>

#include "./State.h"

class Init : public State {
 public:
  void update(double deltaTime) override;
  void draw() override {
    // No draw method
  }
};

#endif  // TANKS_INIT_HPP
