#ifndef SRC_UI_BUTTON_H_
#define SRC_UI_BUTTON_H_

#include <allegro.h>
#include <string>

#include "../input/mouseListener.h"
#include "../util/tools.h"

class Button {
 public:
  // Constructor
  Button(int x, int y, const std::string& text, FONT* button_font);

  Button() = default;

  // Silly Allan! You gotta return padding times two, because there's two sides
  // of padding I gotchu tho, fixed up and respaced buttons in the menu. No
  // weird hitboxes I wonder if you'll ever see this or I'm typing in vain,
  // pretty much my entire life summed up
  int getWidth() const { return width + padding_x * 2; }
  int getHeight() const { return height + padding_y * 2; }

  bool setFont(FONT* font);

  bool clicked() const;
  void update();

  // Draw
  void draw(BITMAP* tempBitmap) const;

 private:
  // Variables
  int x = 0;
  int y = 0;
  int width;
  int height;
  int padding_x = 10;
  int padding_y = 10;
  std::string text = "";

  bool hovering = false;
  bool visible = true;

  // Font
  FONT* button_font = nullptr;
};

#endif  // SRC_UI_BUTTON_H_
