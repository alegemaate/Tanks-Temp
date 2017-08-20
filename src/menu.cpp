#include "menu.h"

// Initilizer
menu::menu(){
  // Background image
  background = load_bitmap_ex( "images/menu.png");

  // Buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);
}

// Update routine
void menu::update(){
  // To the game
  if( key[KEY_SPACE])
    set_next_state( STATE_GAME);
}

// Drawing routine
void menu::draw(){
  // Background
  draw_sprite( buffer, background, 0, 0);

  // Buttons

  // Draw to screen
  draw_sprite( screen, buffer, 0, 0);
}
