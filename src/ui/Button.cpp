#include "Button.hpp"

// Constructor
Button::Button(int x, int y, const std::string& text, FONT* button_font)
    : x(x), y(y), text(text) {
  setFont(button_font);
}

// Set new font
bool Button::setFont(FONT* font) {
  this->button_font = font;
  if (button_font != nullptr) {
    this->width = text_length(button_font, text.c_str());
    this->height = text_height(button_font);
    return true;
  }
  return false;
}

// Update
void Button::update() {
  hovering = mouse_x > x && mouse_x < x + getWidth() && mouse_y > y &&
             mouse_y < y + getHeight();
}

// True if clicked
bool Button::clicked() const {
  return hovering && MouseListener::mouse_pressed & 1;
}

// Draw
void Button::draw(BITMAP* tempBitmap) const {
  if (visible) {
    // Backdrop
    const int c_element = hovering ? 220 : 200;

    rectfill(tempBitmap, x, y, x + width + padding_x * 2,
             y + height + padding_y * 2,
             makecol(c_element, c_element, c_element));
    rect(tempBitmap, x, y, x + width + padding_x * 2,
         y + height + padding_y * 2, makecol(0, 0, 0));

    // Text
    if (button_font != nullptr) {
      textprintf_ex(tempBitmap, button_font, x + padding_x, y + padding_y,
                    makecol(0, 0, 0), -1, text.c_str());
    }
  }
}
