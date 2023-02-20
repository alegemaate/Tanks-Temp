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
int fps;
int frames_done;
int old_time;
const int updates_per_second = 120;
std::array<int, 10> frames_array;
unsigned int frame_index = 0;

void ticker() {
  ticks += 1;
}
END_OF_FUNCTION(ticker)

volatile int game_time = 0;
void game_time_ticker() {
  game_time += 1;
}
END_OF_FUNCTION(game_time_ticker)

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
  install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

  LOCK_VARIABLE(game_time)
  LOCK_FUNCTION(game_time_ticker)
  install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

  // FPS STUFF
  for (unsigned long i = 0; i < 10; i++) {
    frames_array[i] = 0;
  }

  // Close button
  LOCK_FUNCTION(close_button_handler)
  set_close_button_callback(close_button_handler);

  // Set the current state ID
  StateEngine::setNextState(StateId::STATE_INIT);
}

void update() {
  // Change state (if needed)
  StateEngine::changeState();

  // Update listeners
  MouseListener::update();
  KeyListener::update();

  // Update state
  StateEngine::update();
}

int main() {
  // Setup
  setup();

  // FPS Counter
  while (!key[KEY_ESC] && !closing && !joy[0].button[7].b) {
    while (ticks == 0) {
      rest(1);
    }
    while (ticks > 0) {
      int old_ticks = ticks;
      update();
      ticks -= 1;
      if (old_ticks <= ticks) {
        break;
      }
    }
    if (game_time >= old_time + 1) {
      fps -= frames_array[frame_index];
      frames_array[frame_index] = frames_done;
      fps += frames_done;
      frame_index = (frame_index + 1) % 10;
      frames_done = 0;
      old_time += 1;
    }
    StateEngine::draw();
    frames_done++;
  }

  // Exit allegro
  allegro_exit();

  return 0;
}
END_OF_MAIN()
