#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <vector>

#include "../include/barrier.h"
#include "../include/tools.h"
#include "../include/tank.h"

using namespace std;

// Images
BITMAP *buffer;
BITMAP *background;
BITMAP *cursor;
BITMAP *blocks[3];


// Objects
vector<barrier> barriers;
vector<ai_tank> enemy_tanks;
vector<player_tank> player_tanks;

// FPS Tickers
volatile int ticks = 0;
int fps;
int frames_done;
int old_time;
const int updates_per_second = 120;
int frames_array[10];
int frame_index = 0;

void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)

volatile int game_time = 0;
void game_time_ticker()
{
	game_time++;
}
END_OF_FUNCTION(game_time_ticker)


// Close button handler
bool close_button_pressed;
void close_button_handler( void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION( close_button_handler)


// Game update
void update(){
  // Move
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( enemy_tanks.at(i).getBullets());

    // Update tanks
    enemy_tanks.at(i).update();
    // Delete tank
    if(enemy_tanks.at(i).getErase())
      enemy_tanks.erase(enemy_tanks.begin() + i);
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( player_tanks.at(i).getBullets());

    // Update tanks
    player_tanks.at(i).update();
    // Delete tank
    if(player_tanks.at(i).getErase())
      player_tanks.erase(player_tanks.begin() + i);
  }

  // Spawn tank
  if( key[KEY_Z]){
    BITMAP *tank_base_red, *tank_turret_red, *tank_hurt;
    if (!(tank_base_red = load_bitmap( "images/tank_base_red.png", NULL)))
      abort_on_error( "Cannot find image images/tank_base_red.png\nPlease check your files and try again");

    if (!(tank_turret_red = load_bitmap( "images/tank_turret_red.png", NULL)))
      abort_on_error( "Cannot find image images/tank_turret_red.png\nPlease check your files and try again");

    if (!(tank_hurt = load_bitmap( "images/tank_hurt.png", NULL)))
      abort_on_error( "Cannot find image images/tank_hurt.png\nPlease check your files and try again");

    ai_tank newPlayer( random(0, SCREEN_W), random(0, SCREEN_H), 3, random(50,150), random(1,4), random(50,300), random(1,10)/10, tank_base_red, tank_turret_red, tank_hurt);
    enemy_tanks.push_back( newPlayer);
    while(key[KEY_Z]){}
  }
}

void draw(){
  // Draw background
  draw_sprite( buffer, background, 0, 0);

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++)
    barriers.at(i).draw( buffer);

  // Draw tanks
  for( unsigned int i = 0; i < enemy_tanks.size(); i++)
    enemy_tanks.at(i).draw( buffer);
  for( unsigned int i = 0; i < player_tanks.size(); i++)
    player_tanks.at(i).draw( buffer);

  // Cursor
  draw_sprite( buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}

// Setup game
void setup(){
  // Create buffer
  buffer = create_bitmap( 800, 600);

  // Create random number generator
  srand( time( NULL));

   // Setup for FPS system
  LOCK_VARIABLE( ticks);
  LOCK_FUNCTION( ticker);
  install_int_ex( ticker, BPS_TO_TIMER( updates_per_second));

  LOCK_VARIABLE( game_time);
  LOCK_FUNCTION( game_time_ticker);
  install_int_ex( game_time_ticker, BPS_TO_TIMER(10));

  // Close button
  LOCK_FUNCTION( close_button_handler);
  set_close_button_callback( close_button_handler);

  // Load images
  if (!(background = load_bitmap( "images/background.png", NULL)))
    abort_on_error( "Cannot find image images/background.png\nPlease check your files and try again");

  if (!(cursor = load_bitmap( "images/cursor.png", NULL)))
    abort_on_error( "Cannot find image images/cursor.png\nPlease check your files and try again");\

  if (!(blocks[0] = load_bitmap( "images/block_box_1.png", NULL)))
    abort_on_error( "Cannot find image images/block_box_1.png\nPlease check your files and try again");

  if (!(blocks[1] = load_bitmap( "images/block_stone_1.png", NULL)))
    abort_on_error( "Cannot find image images/block_stone_1.png\nPlease check your files and try again");

  if (!(blocks[2] = load_bitmap( "images/block_stone_2.png", NULL)))
    abort_on_error( "Cannot find image images/block_stone_2.png\nPlease check your files and try again");

  // Create barriers
  for( int i = 0; i < 10; i++){
    barrier newBarrier( random(0, SCREEN_W), random( 0, SCREEN_H), blocks[random(0,2)]);
    barriers.push_back( newBarrier);
  }

  // Temp images
  BITMAP *tank_base_green, *tank_turret_green, *tank_base_red, *tank_turret_red, *tank_hurt;
  if (!(tank_base_green = load_bitmap( "images/tank_base_green.png", NULL)))
    abort_on_error( "Cannot find image images/tank_base_green.png\nPlease check your files and try again");

  if (!(tank_turret_green = load_bitmap( "images/tank_turret_green.png", NULL)))
    abort_on_error( "Cannot find image images/tank_turret_green.png\nPlease check your files and try again");

  if (!(tank_base_red = load_bitmap( "images/tank_base_red.png", NULL)))
    abort_on_error( "Cannot find image images/tank_base_red.png\nPlease check your files and try again");

  if (!(tank_turret_red = load_bitmap( "images/tank_turret_red.png", NULL)))
    abort_on_error( "Cannot find image images/tank_turret_red.png\nPlease check your files and try again");

  if (!(tank_hurt = load_bitmap( "images/tank_hurt.png", NULL)))
    abort_on_error( "Cannot find image images/tank_hurt.png\nPlease check your files and try again");

  // Player
  player_tank newPlayer( 50, 50, 3, 100, 4, 0, 1, tank_base_green, tank_turret_green, tank_hurt);
  player_tanks.push_back( newPlayer);

  ai_tank newPlayer2( 400, 400, 3, 0, 2, 10, 1, tank_base_red, tank_turret_red, tank_hurt);
  enemy_tanks.push_back( newPlayer2);

  ai_tank newPlayer3( 200, 200, 3, 100, 3, 20, 1, tank_base_red, tank_turret_red, tank_hurt);
  enemy_tanks.push_back( newPlayer3);

  ai_tank newPlayer4( 500, 300, 3, 100, 3, 100, 1, tank_base_red, tank_turret_red, tank_hurt);
  enemy_tanks.push_back( newPlayer4);

  for(int i = 0; i < 10; i++)
    frames_array[i] = 0;
}

int main(){
  // Init Allegro
  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);

  // Setup screen
  set_gfx_mode( GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, ".");

  // Window Title
  set_window_title( "Tanks!");

  // Setup
  setup();

  // FPS Counter
  while( !key[KEY_ESC] && !close_button_pressed){
    while( ticks == 0){
      rest( 1);
    }
    while( ticks > 0){
      int old_ticks = ticks;

      update();

      ticks--;
      if( old_ticks <= ticks){
        break;
      }
    }
    if(game_time >= old_time + 1){//i.e. a 0.1 second has passed since we last counted the frames{
			fps -= frames_array[frame_index];//decrement the fps by the frames done a second ago
			frames_array[frame_index] = frames_done;//store the number of frames done this 0.1 second
			fps += frames_done;//increment the fps by the newly done frames
			frame_index = (frame_index + 1) % 10;//increment the frame index and snap it to 10
			frames_done = 0;
			old_time += 1;
		}
    draw();
    frames_done++;
  }
	return 0;
}
END_OF_MAIN()
