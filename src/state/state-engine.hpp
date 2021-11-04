
#ifndef SRC_STATE_STATE_ENGINE_HPP_
#define SRC_STATE_STATE_ENGINE_HPP_

#include <memory>
#include "state.h"

// Game states
enum class StateId {
  STATE_NULL,
  STATE_INIT,
  STATE_MENU,
  STATE_GAME,
  STATE_EXIT,
};

// State
class StateEngine {
 public:
  static void setNextState(StateId state_id);

  static void update();

  static void draw();

  static void changeState();

 private:
  static StateId state_id;
  static StateId next_state;
  static std::unique_ptr<State> current_state;
};

#endif  // SRC_STATE_STATE_ENGINE_HPP_
