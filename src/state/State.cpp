#include "State.hpp"

#include <iostream>

#include "Game.hpp"
#include "Init.hpp"
#include "Menu.hpp"

/*****************
 * STATE ENGINE
 *****************/

// Draw
void StateEngine::draw() {
  if (state) {
    // Clear screen
    asw::display::clear();

    state->draw();

    // Update screen
    asw::display::present();
  }
}

// Update
void StateEngine::update(const double deltaTime) {
  if (state) {
    state->update(deltaTime);
  }

  changeState();
}

// Set next state
void StateEngine::setNextState(const ProgramState newState) {
  nextState = newState;
}

// Get state id
auto StateEngine::getStateId() const -> ProgramState {
  return currentState;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == ProgramState::Null) {
    return;
  }

  // Delete the current state
  if (state) {
    state->cleanup();
    state = nullptr;
  }

  // Change the state
  switch (nextState) {
    case ProgramState::Game:
      state = std::make_unique<Game>(*this);
      std::cout << "Switched state to game." << std::endl;
      break;

    case ProgramState::Menu:
      state = std::make_unique<Menu>(*this);
      std::cout << "Switched state to main menu." << std::endl;
      break;

    case ProgramState::Init:
      state = std::make_unique<Init>(*this);
      std::cout << "Switched state to init." << std::endl;
      break;

    default:
      std::cout << "Exiting progra" << std::endl;
      break;
  }

  state->init();

  // Change the current state ID
  currentState = nextState;

  // NULL the next state ID
  nextState = ProgramState::Null;
}

/*********
 * STATE
 *********/

// Change state
void State::setNextState(const ProgramState state) {
  this->engine.setNextState(state);
}
