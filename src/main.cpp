#include <allegro.h>

#include <array>

#include "./input/KeyListener.hpp"
#include "./input/MouseListener.hpp"

#include "./state/Game.hpp"
#include "./state/StateEngine.hpp"

// Are we closing?
bool closing = false;

// FPS Tickers
volatile int ticks = 0;
void ticker() {
  ticks += 1;
}
END_OF_FUNCTION(ticker)

// Close button handler
void close_button_handler() {
  closing = true;
}
END_OF_FUNCTION(close_button_handler)

// Calibrate joystick
bool calibrateJoystick() {
  for (int i = 0; i < num_joysticks; i++) {
    while (joy[i].flags & JOYFLAG_CALIBRATE) {
      if ((readkey() & 0xFF) == 27) {
        return false;
      }

      if (calibrate_joystick(i) != 0) {
        return false;
      }
    }
    if (!(joy[i].stick[0].flags & JOYFLAG_ANALOGUE)) {
      allegro_message(
          "This game only supports analogue joysticks, please unplug and try "
          "again. \n");
    }
  }

  save_joystick_data("joy_config.dat");
  return true;
}

// Setup game
void setup() {
  // Init Allegro
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();

  // Setup joystick
  if (!load_joystick_data("joy_config.dat")) {
    install_joystick(JOY_TYPE_AUTODETECT);
    if (!calibrateJoystick()) {
      abort_on_error("Could not configure joystick");
    }
  }

  set_color_depth(32);

  // Setup screen
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".");

  // Window Title
  set_window_title("Tanks!");

  // Setup for FPS system
  LOCK_VARIABLE(ticks)
  LOCK_FUNCTION(ticker)
  install_int_ex(ticker, BPS_TO_TIMER(1000));

  // Close button
  LOCK_FUNCTION(close_button_handler)
  set_close_button_callback(close_button_handler);

  // Set the current state ID
  StateEngine::setNextState(StateId::STATE_INIT);
  StateEngine::changeState();
}

void update(const double deltaTime) {
  // Change state (if needed)
  StateEngine::changeState();

  // Update listeners
  MouseListener::update();
  KeyListener::update();

  // Update state
  StateEngine::update(deltaTime);
}

int main() {
  // Setup
  setup();

  // 120 Updates per second
  const constexpr double dt = 1000.0 / 120.0;

  double time = 0.0;
  double accumulator = 0.0;
  double current_time = ticks;
  double new_time = 0.0;
  double frame_time = 0.0;

  while (!key[KEY_ESC] && !closing) {
    new_time = ticks;
    frame_time = new_time - current_time;
    current_time = new_time;

    accumulator += frame_time;

    while (accumulator >= dt) {
      update(accumulator);

      accumulator -= dt;
      time += dt;
    }

    StateEngine::draw();
  }

  // Exit allegro
  allegro_exit();

  return 0;
}
END_OF_MAIN()
