#include <asw/asw.h>
#include <chrono>
#include <iostream>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "./state/Game.hpp"
#include "./state/State.hpp"

using namespace std::chrono_literals;
using namespace std::chrono;
constexpr nanoseconds timestep(16ms);

// State engine
std::unique_ptr<StateEngine> game_state;

// // Calibrate joystick TODO
// bool calibrateJoystick() {
//   for (int i = 0; i < num_joysticks; i++) {
//     while (joy[i].flags & JOYFLAG_CALIBRATE) {
//       if ((readkey() & 0xFF) == 27) {
//         return false;
//       }

//       if (calibrate_joystick(i) != 0) {
//         return false;
//       }
//     }
//     if (!(joy[i].stick[0].flags & JOYFLAG_ANALOGUE)) {
//       allegro_message(
//           "This game only supports analogue joysticks, please unplug and try
//           " "again. \n");
//     }
//   }

//   save_joystick_data("joy_config.dat");
//   return true;
// }

// Setup game
void setup() {
  // Load asw library
  asw::core::init(800, 600);

  game_state = std::make_unique<StateEngine>();

  // Set the current state ID
  game_state->setNextState(ProgramState::Init);
}

void update(const double deltaTime) {
  // Update listeners
  asw::core::update();

  // Do state logic
  game_state->update(deltaTime);

  // Handle exit
  if (game_state->getStateId() == ProgramState::Exit) {
    asw::core::exit = true;
  }
}

// Do state rendering
void draw() {
  game_state->draw();
}

// Loop (emscripten compatibility)
#ifdef __EMSCRIPTEN__
void loop() {
  update(timestep / 1ms);
  draw();
}
#endif

// Main function*/
auto main(int argc, char* argv[]) -> int {
  // Setup basic functionality
  setup();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
#else

  using clock = high_resolution_clock;
  nanoseconds lag(0ns);
  auto time_start = clock::now();
  auto last_second = clock::now();
  int frames = 0;
  int fps = 0;

  while (!asw::input::isKeyDown(asw::input::Key::ESCAPE) && !asw::core::exit) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += duration_cast<nanoseconds>(delta_time);

    while (lag >= timestep) {
      update(timestep / 1ms);
      lag -= timestep;
    }

    draw();
    frames++;

    if (clock::now() - last_second >= 1s) {
      fps = frames;
      frames = 0;
      last_second = last_second + 1s;
      asw::display::setTitle("Tanks - FPS: " + std::to_string(fps));
    }
  }

#endif

  return 0;
}
