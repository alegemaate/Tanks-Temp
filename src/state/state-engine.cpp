#include "state-engine.hpp"

#include <iostream>
#include "game.h"
#include "init.h"
#include "menu.h"

// State variables
StateId StateEngine::state_id = StateId::STATE_NULL;
StateId StateEngine::next_state = StateId::STATE_NULL;
State* StateEngine::current_state = nullptr;

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
    delete current_state;
  }

  // Change the state
  switch (next_state) {
    case StateId::STATE_INIT:
      current_state = new Init();
      break;
    case StateId::STATE_GAME:
      current_state = new Game();
      break;
    case StateId::STATE_MENU:
      current_state = new Menu();
      break;
    case StateId::STATE_EXIT:
    case StateId::STATE_NULL:
      // closing = true;
      break;
  }

  // Change the current state ID
  state_id = next_state;

  // NULL the next state ID
  next_state = StateId::STATE_NULL;
}
