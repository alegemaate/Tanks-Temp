#include "state-engine.hpp"

#include "game.h"
#include "init.h"
#include "menu.h"

// State variables
StateId StateEngine::state_id = StateId::STATE_NULL;
StateId StateEngine::next_state = StateId::STATE_NULL;
std::unique_ptr<State> StateEngine::current_state = nullptr;

// Set next state
void StateEngine::setNextState(StateId state_id) {
  next_state = state_id;
}

void StateEngine::update() {
  current_state->update();
}

void StateEngine::draw() {
  current_state->draw();
}

void StateEngine::changeState() {
  // If the state needs to be changed
  if (next_state == StateId::STATE_NULL) {
    return;
  }

  // Delete the current state
  if (next_state != StateId::STATE_EXIT) {
    current_state.reset();
  }

  // Change the state
  switch (next_state) {
    case StateId::STATE_INIT:
      current_state = std::make_unique<Init>();
      break;
    case StateId::STATE_GAME:
      current_state = std::make_unique<Game>();
      break;
    case StateId::STATE_MENU:
      current_state = std::make_unique<Menu>();
      break;
    default:
      break;
  }

  // Change the current state ID
  state_id = next_state;

  // NULL the next state ID
  next_state = StateId::STATE_NULL;
}
