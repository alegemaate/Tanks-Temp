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
BITMAP *decal_buffer;
BITMAP *background;
BITMAP *cursor;
BITMAP *blocks[3];

BITMAP *tank_images[10];


// Objects
vector<barrier> barriers;
vector<tank*> enemy_tanks;
vector<tank*> player_tanks;

// Map stuff
const int map_width = 800/40;
const int map_height = 600/40;
int map_temp[map_width][map_height];

// FPS Tickers
volatile int ticks = 0;
int fps;
int frames_done;
int old_time;
const int updates_per_second = 120;
int frames_array[10];
int frame_index = 0;

int currentRound = 0;

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


// Calibrate joystick
void calibrateJoystick(){
  for( int i = 0; i < num_joysticks; i ++){
    while (joy[i].flags & JOYFLAG_CALIBRATE) {
      //AL_CONST char *msg = calibrate_joystick_name(i);

      if ((readkey()&0xFF) == 27)
        exit(0);

      if (calibrate_joystick(i) != 0) {
         set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
         allegro_message("Error calibrating joystick!\n");
         exit(1);
      }
    }
    if (!(joy[i].stick[0].flags & JOYFLAG_ANALOGUE))
      allegro_message("This game only supports analogue joysticks, please unplug and try again. \n");
  }

  save_joystick_data("joy_config.dat");
}

// Coordinate system
struct coordinate{
  int x;
  int y;

  coordinate(){
    x = 0;
    y = 0;
  }
};
vector<coordinate> startLocations;

// Game update
void update(){
  // Get joystick input
  poll_joystick();

  // Move
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( enemy_tanks.at(i) -> getBullets());

    // Update bullets
    for( unsigned int t = 0; t < player_tanks.size(); t++)
      player_tanks.at(t) -> checkCollision( enemy_tanks.at(i) -> getBullets());

    // Collision with barrier
    enemy_tanks.at(i) -> checkCollision( &barriers);

    // Update tanks
    enemy_tanks.at(i) -> update();

    // Delete tank
    if(enemy_tanks.at(i) -> getErase()){
      delete enemy_tanks[i];
      enemy_tanks.erase(enemy_tanks.begin() + i);
    }
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( player_tanks.at(i) -> getBullets());

    // Update bullets
    for( unsigned int t = 0; t < enemy_tanks.size(); t++)
      enemy_tanks.at(t) -> checkCollision( player_tanks.at(i) -> getBullets());

    // Collision with barrier
    player_tanks.at(i) -> checkCollision( &barriers);

    // Update tanks
    player_tanks.at(i) -> update();

    // Delete tank
    if(player_tanks.at(i) -> getErase()){
      delete player_tanks[i];
      player_tanks.erase(player_tanks.begin() + i);
    }
  }

  // Remove broken barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    if( barriers.at(i).getDead()){
      barriers.erase( barriers.begin() + i);
    }
  }

  // GAME!

  // Next round
  if( enemy_tanks.size() == 0){
    currentRound += 1;

    for( int i = 0; i < currentRound; i ++){
      // choose a start location ID
      int randomStartLocation = random( 0, startLocations.size() - 1);

      ai_tank *newPlayer = new ai_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                        random(50,150), random(1,4), random(50,300), random(1,10)/10,
                        tank_images[5], tank_images[4], tank_images[1], tank_images[0]);
      newPlayer -> process_enemies( &player_tanks);
      enemy_tanks.push_back( newPlayer);
    }
  }
  // U died
  else if( player_tanks.size() == 0){
    enemy_tanks.clear();
    player_tanks.clear();
    currentRound = 0;

    // The new you!
    int randomStartLocation = random( 0, startLocations.size());

    player_tank *newPlayer = new player_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                            100, 4, 20, 1,
                            tank_images[3], tank_images[2], tank_images[1], tank_images[0]);

    newPlayer -> process_enemies( &enemy_tanks);
    player_tanks.push_back( newPlayer);

    // Friends?
    for( int i = 0; i < 5; i ++){
      ai_tank *newPlayer = new ai_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                            100, 4, 20, 1,
                            tank_images[7], tank_images[6], tank_images[1], tank_images[0]);

      newPlayer -> process_enemies( &enemy_tanks);
      player_tanks.push_back( newPlayer);
    }
  }
}

void draw(){
  // Draw background
  draw_sprite( buffer, background, 0, 0);

  // Decal to buffer
  draw_sprite( buffer, decal_buffer, 0, 0);

  // Draw tanks
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    enemy_tanks.at(i) -> draw( buffer);
    if( random( 1, 3) == 1)
      enemy_tanks.at(i) -> putDecal( decal_buffer);
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    player_tanks.at(i) -> draw( buffer);
    if( random( 1, 3) == 1)
      player_tanks.at(i) -> putDecal( decal_buffer);
  }

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++)
    barriers.at(i).draw( buffer);

  // Cursor
  draw_sprite( buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}

// Setup game
void setup(){
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

  // Setup joystick
  if( !load_joystick_data("joy_config.dat")){
    install_joystick(JOY_TYPE_AUTODETECT);
    calibrateJoystick();
  }

  // Window Title
  set_window_title( "Tanks!");

  // Create buffer
  buffer = create_bitmap( 800, 600);
  decal_buffer = create_bitmap( 800, 600);
  rectfill( decal_buffer, 0, 0, 800, 600, makecol( 255, 0, 255));

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

  if (!(blocks[2] = load_bitmap( "images/block_box_1.png", NULL)))
    abort_on_error( "Cannot find image images/block_box_1.png\nPlease check your files and try again");


  if (!(tank_images[0] = load_bitmap( "images/tank_treads.png", NULL)))
    abort_on_error( "Cannot find image images/tank_treads.png\nPlease check your files and try again");

  if (!(tank_images[1] = load_bitmap( "images/tank_dead.png", NULL)))
    abort_on_error( "Cannot find image images/tank_dead.png\nPlease check your files and try again");

  if (!(tank_images[2] = load_bitmap( "images/tank_turret_green.png", NULL)))
    abort_on_error( "Cannot find image images/tank_turret_green.png\nPlease check your files and try again");

  if (!(tank_images[3] = load_bitmap( "images/tank_base_green.png", NULL)))
    abort_on_error( "Cannot find image images/tank_base_green.png\nPlease check your files and try again");

  if (!(tank_images[4] = load_bitmap( "images/tank_turret_red.png", NULL)))
    abort_on_error( "Cannot find image images/tank_turret_red.png\nPlease check your files and try again");

  if (!(tank_images[5] = load_bitmap( "images/tank_base_red.png", NULL)))
    abort_on_error( "Cannot find image images/tank_base_red.png\nPlease check your files and try again");

  if (!(tank_images[6] = load_bitmap( "images/tank_turret_blue.png", NULL)))
    abort_on_error( "Cannot find image images/tank_turret_blue.png\nPlease check your files and try again");

  if (!(tank_images[7] = load_bitmap( "images/tank_base_blue.png", NULL)))
    abort_on_error( "Cannot find image images/tank_base_blue.png\nPlease check your files and try again");

  // Make a map
  // Erase map
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      map_temp[i][t] = 0;
    }
  }
  // Pass 1 (Edges)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1){
        map_temp[i][t] = 1;
      }
    }
  }
  // Pass 2 (Well Placed blocks)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
               map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
               map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
               map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
               random( 0, 2) == 1){
        map_temp[i][t] = 1;
      }
    }
  }
  // Pass 3 (Filling)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( (map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1) ||
          (map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1)){
        map_temp[i][t] = 1;
      }
    }
  }
  // Pass 4 (Filling Unaccessable areas)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1 &&
          map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1){
        map_temp[i][t] = 1;
      }
    }
  }
  // Pass 5 (Boxes!)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( map_temp[i][t] == 0 && random( 1, 20) == 1){
        map_temp[i][t] = 2;
      }
    }
  }
  // Find start locations
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( map_temp[i][t] == 0){
        coordinate newStartLocation;
        newStartLocation.x = i * 40;
        newStartLocation.y = t * 40;
        startLocations.push_back( newStartLocation);
      }
    }
  }

  // Create barriers (where needed)
  for( int i = 0; i < map_width; i++){
    for( int t = 0; t < map_height; t++){
      if( map_temp[i][t] != 0){
        barrier newBarrier( i * 40, t * 40, blocks[map_temp[i][t]], -1);

        // Destroyable
        if( map_temp[i][t] == 2)
          newBarrier.setHealth(3);

        barriers.push_back( newBarrier);
      }
    }
  }

  // FPS STUFF
  for(int i = 0; i < 10; i++)
    frames_array[i] = 0;
}

int main(){
  // Setup
  setup();

  // FPS Counter
  while( !key[KEY_ESC] && !close_button_pressed && !joy[0].button[7].b){
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
    if(game_time >= old_time + 1){// i.e. a 0.1 second has passed since we last counted the frames{
			fps -= frames_array[frame_index];// decrement the fps by the frames done a second ago
			frames_array[frame_index] = frames_done;// store the number of frames done this 0.1 second
			fps += frames_done;// increment the fps by the newly done frames
			frame_index = (frame_index + 1) % 10;// increment the frame index and snap it to 10
			frames_done = 0;
			old_time += 1;
		}
    draw();
    frames_done++;
  }
	return 0;
}
END_OF_MAIN()
