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

class keyListener {
 public:
  keyListener();
  virtual ~keyListener();

  static bool anyKeyPressed;

  void update();

  static bool keyPressed[KEY_MAX];
  static bool keyReleased[KEY_MAX];

  static int lastKeyPressed;
  static int lastKeyReleased;

 protected:
 private:
  static bool lastTicksKey[KEY_MAX];
};

#endif  // SRC_INPUT_KEYLISTENER_H_
