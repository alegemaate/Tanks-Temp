/**
 * Key Listener
 *   Allan Legemaate
 * Listens for keys JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
 **/

#ifndef SRC_INPUT_KEYLISTENER_H_
#define SRC_INPUT_KEYLISTENER_H_

#include <allegro.h>
#include <array>

class KeyListener {
 public:
  static bool anyKeyPressed;

  static void update();

  static std::array<bool, KEY_MAX> keyPressed;
  static std::array<bool, KEY_MAX> keyReleased;

  static int lastKeyPressed;
  static int lastKeyReleased;

 private:
  static std::array<bool, KEY_MAX> lastTicksKey;
};

#endif  // SRC_INPUT_KEYLISTENER_H_
