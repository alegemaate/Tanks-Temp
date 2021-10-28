#include "button.h"

// Defaults
Button::Button() {
  this->x = 0;
  this->y = 0;
  this->text = "";
  this->image = nullptr;
  this->button_font = nullptr;
  this->hovering = false;

  padding_x = 10;
  padding_y = 10;
}

// Constructor
Button::Button(int x,
               int y,
               std::string text,
               FONT* button_font,
               int width,
               int height,
               int padding_x,
               int padding_y) {
  this->x = x;
  this->y = y;
  this->text = text;
  this->image = nullptr;

  if (!setFont(button_font)) {
    this->width = width;
    this->height = height;
  }

  this->visible = true;
  this->hovering = false;

  this->padding_x = padding_x;
  this->padding_y = padding_y;
}

// Destruct
Button::~Button() {
  if (image != nullptr) {
    // destroy_bitmap(image);
  }
}

// Sets an image
void Button::setImage(BITMAP* image) {
  this->image = image;
  this->width = this->image->w;
  this->height = this->image->h;
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

// True if hovering
bool Button::hover() {
  return hovering;
}

// True if clicked
bool Button::clicked() {
  return hovering && mouseListener::mouse_pressed & 1;
}

// Draw
void Button::draw(BITMAP* tempBitmap) {
  if (visible) {
    // Backdrop
    rectfill(
        tempBitmap, x, y, x + width + padding_x * 2, y + height + padding_y * 2,
        makecol(200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering));
    rect(tempBitmap, x, y, x + width + padding_x * 2,
         y + height + padding_y * 2, makecol(0, 0, 0));

    // Text
    if (button_font != nullptr)
      textprintf_ex(tempBitmap, button_font, x + padding_x, y + padding_y,
                    makecol(0, 0, 0), -1, text.c_str());

    // Image if avail
    if (image != nullptr)
      draw_sprite(tempBitmap, image, x + padding_x, y + padding_y);
  }
}
