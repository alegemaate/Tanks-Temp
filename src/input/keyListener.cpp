#include "keyListener.h"

std::array<bool, KEY_MAX> KeyListener::keyPressed = {false};
std::array<bool, KEY_MAX> KeyListener::keyReleased = {false};
std::array<bool, KEY_MAX> KeyListener::lastTicksKey = {false};
int KeyListener::lastKeyPressed = -1;
int KeyListener::lastKeyReleased = -1;
bool KeyListener::anyKeyPressed = false;

// Check those keys!
void KeyListener::update() const {
  // Reset last key
  lastKeyPressed = -1;
  lastKeyReleased = -1;
  anyKeyPressed = false;

  // Check key just pressed
  for (int i = 0; i < KEY_MAX; i++) {
    // Clear old values
    keyPressed[i] = false;
    keyReleased[i] = false;

    // Wheres the any key?
    if (static_cast<bool>(key[i])) {
      anyKeyPressed = true;
    }

    // Pressed since last tick?
    if (static_cast<bool>(key[i]) == true && lastTicksKey[i] == false) {
      keyPressed[i] = true;
      lastKeyPressed = i;
    }

    // Released since last tick?
    if (static_cast<bool>(key[i]) == false && lastTicksKey[i] == true) {
      keyReleased[i] = true;
      lastKeyReleased = i;
    }
  }

  // Get new values
  for (int i = 0; i < KEY_MAX; i++) {
    // Key changed
    if (lastTicksKey[i] != static_cast<bool>(key[i])) {
      lastTicksKey[i] = key[i];
    }
  }
}
