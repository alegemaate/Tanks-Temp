#include "menu.h"

// Initilizer
menu::menu(){
  // Background image
  background = load_bitmap_ex( "images/menu.png");

  // Buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Buttons
  enemies_up = button( 90, 275, "/\\", font);
  enemies_down = button( 90, 320, "\\/", font);
  friends_up = button( 460, 275, "/\\", font);
  friends_down = button( 460, 320, "\\/", font);
  start = button( 340, 485, "START", font);
}

// Update routine
void menu::update(){
  // Update buttons
  enemies_up.update();
  enemies_down.update();
  friends_up.update();
  friends_down.update();
  start.update();

  // Make teams
  if( enemies_up.clicked())
    num_enemies ++;
  if( enemies_down.clicked())
    num_enemies --;
  if( friends_up.clicked())
    num_friends ++;
  if( friends_down.clicked())
    num_friends --;

  // Start game
  if( start.clicked())
    set_next_state( STATE_GAME);
}

// Drawing routine
void menu::draw(){
  // Background
  draw_sprite( buffer, background, 0, 0);

  // Buttons
  enemies_up.draw( buffer);
  enemies_down.draw( buffer);
  friends_up.draw( buffer);
  friends_down.draw( buffer);
  start.draw( buffer);

  // Player nums
  textprintf_ex( buffer, font, 150, 300, makecol( 0, 0, 0), -1, "%i", num_enemies);
  textprintf_ex( buffer, font, 530, 300, makecol( 0, 0, 0), -1, "%i", num_friends);

  // Mouse
  rectfill( buffer, mouse_x, mouse_y, mouse_x + 4, mouse_y + 4, makecol( 255, 255, 255));

  // Draw to screen
  draw_sprite( screen, buffer, 0, 0);
}
