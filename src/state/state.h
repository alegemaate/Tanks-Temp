/**
 * State machine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program
 */
#ifndef SRC_STATE_STATE_H_
#define SRC_STATE_STATE_H_

class State {
 public:
  // Deconstructor
  virtual ~State() {}

  // Draw to screen
  virtual void draw() = 0;

  // Update logic
  virtual void update() = 0;
};

#endif  // SRC_STATE_STATE_H_
