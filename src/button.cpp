#include "button.h"


// Defaults
button::button(){
  this -> x = 0;
  this -> y = 0;
  this -> text = "";
  this -> image = nullptr;
  this -> button_font = nullptr;
  this -> hovering = false;

  padding_x = 10;
  padding_y = 10;
}

// Constructor
button::button( int x, int y, std::string text, FONT *button_font, int width, int height, int padding_x, int padding_y){

  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> button_font = button_font;

  this -> visible = true;

  this -> mouse_released=false;
  this -> old_mouse_down=false;
  this -> hovering = false;

  if( button_font != nullptr){
    //this -> width = al_get_text_width( button_font, text.c_str());
    //this -> height = al_get_font_line_height( button_font);
  }
  else{
    this -> width = width;
    this -> height = height;
  }

  this -> padding_x = padding_x;
  this -> padding_y = padding_y;
}

// Destruct
button::~button(){
  if( image != nullptr)
    destroy_bitmap( image);
}

// Sets an image
void button::setImage( BITMAP *image){
  this -> image = image;
  this -> width = this -> image -> w;
  this -> height = this -> image -> h;
}

// Set new font
void button::setFont( FONT *font){
  this -> button_font = font;
  if( button_font != nullptr){
    //this -> width = al_get_text_width( button_font, text.c_str());
    //this -> height = al_get_font_line_height( button_font);
  }
}

// Update
void button::update(){
  hovering = mouse_x > x && mouse_x < x + getWidth() &&
             mouse_y > y && mouse_y < y + getHeight();
}

// True if hovering
bool button::hover(){
  return hovering;
}

// True if clicked
bool button::clicked(){
  return hovering && mouseListener::mouse_pressed & 1;
}

bool button::mouseReleased(){
  return mouse_released;
}

// Draw
void button::draw( BITMAP *tempBitmap){
  if(visible){
    // Backdrop
    rectfill( tempBitmap, x, y, x + width + padding_x * 2, y + height + padding_y * 2, makecol( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering));
    rect( tempBitmap, x, y, x + width + padding_x * 2, y + height + padding_y * 2, makecol( 0, 0, 0));

    // Text
    if( button_font != nullptr)
      textprintf_ex( tempBitmap, button_font, makecol( 0, 0, 0), x + padding_x, y + padding_y, 0, text.c_str());

    // Image if avail
    if( image != nullptr)
      draw_sprite( tempBitmap, image, x + padding_x, y + padding_y);
  }
}
