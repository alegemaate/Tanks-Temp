#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <vector>

#include "../include/barrier.h"
#include "../include/tools.h"
#include "../include/bullet.h"

#define PLAYER TRUE
#define HELICOPTER FALSE

#define LEFT 0
#define RIGHT 1
#define HOVER 2

using namespace std;

// Images
BITMAP* buffer;
BITMAP* player;
BITMAP* player_top;
BITMAP* player_hurt;
BITMAP* background;
BITMAP* cursor;

// Sounds
SAMPLE* fire;

// Close button handler
bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 120;
volatile int game_time = 0;

// FPS Variables
int fps;
int frames_done;
int old_time;

// Player variables
float player_x = 400;
float player_y = 300;
int player_hurt_timer;
int player_health = 100;
int player_fire_rate;
int player_fire_delay_rate;
int player_fire_rate_timer;
float player_rotation_radians;
float player_rotation_allegro;
float player_vector_x;
float player_vector_y;
float player_speed;

// Other variables
int bullet_delay;
int helicopter_killcount;

// Mouse angle handlers
float mouse_angle_radians;
float mouse_angle_allegro;

// Objects
vector<bullet> bullets;
vector<barrier> barriers;

// FPS Tickers
void ticker(){
  ticks++;
}
END_OF_FUNCTION( ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION( ticker)

// Close button function
void close_button_handler( void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION( close_button_handler)

// Game update
void update(){
  // Move
  if( mouse_b & 1){
    player_rotation_radians = find_angle( player_x + 15, player_y + 20, mouse_x, mouse_y);
    player_speed = 1;
  }
  else{
    player_speed = 0;
  }

  player_vector_x = -player_speed * cos( player_rotation_radians);
  player_vector_y = -player_speed * sin( player_rotation_radians);
  player_rotation_allegro = player_rotation_radians * 40.5845104792;
  player_x += player_vector_x;
  player_y += player_vector_y;

  // End game when dead
  if( player_health < 1)
    close_button_pressed=true;

  // Change timers
  player_hurt_timer--;
  player_fire_rate_timer--;
  bullet_delay++;

  // Reset timers
  if( player_fire_rate_timer < 1){
    player_fire_rate = 3;
    player_fire_delay_rate = 10;
  }

  // Set shooting angle
  mouse_angle_radians = find_angle( player_x + 15, player_y + 20, mouse_x, mouse_y);
  mouse_angle_allegro = mouse_angle_radians * 40.5845104792;

  // Shoot
  if(( key[KEY_SPACE] || mouse_b & 2) && bullet_delay > player_fire_delay_rate ){
    bullet newBullet( player_x + 15, player_y + 20, mouse_angle_radians, player_fire_rate, PLAYER, 1, fire);
    bullets.push_back( newBullet);
    bullet_delay = 0;
  }

  // Update bullets
  for( unsigned int i = 0; i < bullets.size(); i++){
    bullets.at(i).update();
    if(bullets.at(i).getErase())
      bullets.erase(bullets.begin() + i);
  }

  // Erase bullets
  if( key[KEY_C]){
    bullets.clear();
  }

  // Update barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    barriers.at(i).update( &bullets);
  }
}

void draw(){
  // Draw background
  draw_sprite( buffer, background, 0, 0);

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    barriers.at(i).draw( buffer);
  }

  // Hurt image for player
  if( player_hurt_timer<1){
    rotate_sprite( buffer, player, player_x, player_y, itofix(player_rotation_allegro));
  }
  else{
    rotate_sprite( buffer, player_hurt, player_x, player_y, itofix(player_rotation_allegro));
  }

  // Turret
  rotate_sprite( buffer, player_top, player_x, player_y, itofix(mouse_angle_allegro));

  // Health Bar
  rectfill(buffer,550,10,754,30,makecol(0,0,0));
  rectfill(buffer,552,12,752,28,makecol(255,0,0));
  rectfill(buffer,552,12,552+(player_health*2),28,makecol(0,255,0));

  // Draw bullets
  for( unsigned int i=0; i < bullets.size(); i++){
    bullets.at(i).draw( buffer);
  }

  // Debug
  textprintf_ex(buffer,font,20,20,makecol(0,0,0),-1,"Helicopter Kill Count: %f", player_rotation_allegro);
  textprintf_ex(buffer,font,20,60,makecol(0,0,0),-1,"player_vector_x: %f", player_vector_x);
  textprintf_ex(buffer,font,20,100,makecol(0,0,0),-1,"player_vector_y: %f", player_vector_y);
  textprintf_ex(buffer,font,20,120,makecol(0,0,0),-1,"x: %f", player_x);
  textprintf_ex(buffer,font,20,140,makecol(0,0,0),-1,"y: %f", player_y);

  // Cursor
  draw_sprite(buffer,cursor,mouse_x-10,mouse_y-10);

  // Buffer to screen
  draw_sprite(screen,buffer,0,0);
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
  if (!(player = load_bitmap( "images/player.png", NULL)))
    abort_on_error( "Cannot find image images/player.png\nPlease check your files and try again");

  if (!(player_hurt = load_bitmap( "images/player_hurt.png", NULL)))
    abort_on_error( "Cannot find image images/player_hurt.png\nPlease check your files and try again");

  if (!(player_top = load_bitmap( "images/player_top.png", NULL)))
    abort_on_error( "Cannot find image images/player_top.png\nPlease check your files and try again");

  if (!(background = load_bitmap( "images/background.png", NULL)))
    abort_on_error( "Cannot find image images/background.png\nPlease check your files and try again");

  if (!(cursor = load_bitmap( "images/cursor.png", NULL)))
    abort_on_error( "Cannot find image images/cursor.png\nPlease check your files and try again");\

  // Load sounds
  if (!(fire = load_sample( "sfx/fire.wav")))
    abort_on_error( "Cannot find image sfx/fire.wav\nPlease check your files and try again");

  // Create barriers
  for( int i = 0; i < 10; i++){
    barrier newBarrier( random(0, SCREEN_W), random( 0, SCREEN_H));
    barriers.push_back( newBarrier);
  }
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
    if( game_time - old_time >= 10){
      fps = frames_done;
      frames_done = 0;
      old_time = game_time;
    }
    draw();
  }
	return 0;
}
END_OF_MAIN()
