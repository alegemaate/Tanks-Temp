/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
 **/
#ifndef TANKS_INIT_HPP
#define TANKS_INIT_HPP

#include <asw/asw.h>

#include "./State.hpp"

class Init : public State {
 public:
  explicit Init(StateEngine& engine) : State(engine) {}

  void init() override;
  void update(const float deltaTime) override {
    // Goto menu
    this->setNextState(ProgramState::Menu);
  }
  void draw() override {
    // No draw method
  }
  void cleanup() override {
    // No cleanup
  }
};

#endif  // TANKS_INIT_HPP
