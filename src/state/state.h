/**
 * State machine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program
 */
#ifndef SRC_STATE_STATE_H_
#define SRC_STATE_STATE_H_

// State variables
extern int stateID;
extern int nextState;

// Set next state
extern void set_next_state(int state);

// Game states
enum ProgramStates {
  STATE_NULL,
  STATE_INIT,
  STATE_INTRO,
  STATE_MENU,
  STATE_GAME,
  STATE_EDIT,
  STATE_EXIT,
};

// State
class State {
 public:
  // Deconstructor
  virtual ~State() {}

  int getStateID() { return stateID; }

  // Draw to screen
  virtual void draw() = 0;

  // Update logic
  virtual void update() = 0;
};

#endif  // SRC_STATE_STATE_H_
