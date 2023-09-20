/**
 * State for machine and State Engine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program into states
 *   which can handle only their own logic,
 *   drawing and transitions
 */

#ifndef STATE_H
#define STATE_H

#include <asw/asw.h>
#include <memory>

// Class
class State;

// Game states
enum class ProgramState {
  Null,
  Init,
  Menu,
  Game,
  Exit,
};

/*****************
 * STATE ENGINE
 *****************/
class StateEngine {
 public:
  // Init
  StateEngine() = default;

  // Update
  void update(const double deltaTime);

  // Draw
  void draw();

  // Set next state
  void setNextState(ProgramState state);

  // Get state id
  auto getStateId() const -> ProgramState;

 private:
  // Change state
  void changeState();

  // Next state
  ProgramState nextState{ProgramState::Null};

  // State id
  ProgramState currentState{ProgramState::Null};

  // Stores states
  std::unique_ptr<State> state{nullptr};
};

/*********
 * STATE
 *********/
class State {
 public:
  // Constructor
  explicit State(StateEngine& engine) : engine(engine){};

  virtual ~State() = default;

  // Init the state
  virtual void init() = 0;

  // Draw to screen
  virtual void draw() = 0;

  // Cleanup
  virtual void cleanup() = 0;

  // Update logic
  virtual void update(const double deltaTime) = 0;

  // Change state
  void setNextState(ProgramState state);

 private:
  StateEngine& engine;
};

#endif  // STATE_H
