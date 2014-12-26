#include <allegro.h>
#include <alpng.h>
#include <time.h>
#include <cmath>
#include <vector>

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
BITMAP* bullet_image;
BITMAP* helicopter_image;
BITMAP* helicopter_hurt;

// Close button handler
bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
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
int player_laser_timer;
bool player_is_lasering;
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

// Ray Tracer
struct raytracer{
  float x;
  float y;
  float vector_x;
  float vector_y;
  bool on_screen;

}raytracer[10];

// Bullets
struct bullet{
  float x;
  float y;
  float vector_x;
  float vector_y;
  bool on_screen;
  bool owner;
};

vector<bullet> bullets;

// Boxes
struct boxes{
  int x;
  int y;
  int type;
  bool on_screen;
}box[10];

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

// Random number generator. Use int random(highest,lowest);
int random( int newLowest, int newHighest){
  int lowest = newLowest, highest = newHighest;
  int range = ( highest - lowest) + 1;
  int randomNumber = lowest + int( range * rand() / (RAND_MAX + 1.0));
  return randomNumber;
}

//Collision between 2 boxes
bool collision( float xMin1, float xMax1, float xMin2, float xMax2, float yMin1, float yMax1, float yMin2, float yMax2){
  if ( xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}

//A function to streamline error reporting in file loading
void abort_on_error( const char *message){
  set_window_title( "Error!");
  if ( screen != NULL){
    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
  }
  allegro_message( "%s.\n %s\n", message, allegro_error);
  exit( -1);
}

//Raytracer
void raytrace(){
  player_is_lasering = true;
  int i = 1;
  raytracer[i].x = player_x;
  raytracer[i].y = player_y;
  raytracer[i].vector_x = -1 * cos( mouse_angle_radians);
  raytracer[i].vector_y = -1 * sin( mouse_angle_radians);
  while(raytracer[i].x < 800 && raytracer[i].y < 600 && raytracer[i].x > 0 && raytracer[i].y > 0){
    raytracer[i].x += raytracer[i].vector_x;
    raytracer[i].y += raytracer[i].vector_y;
    for( int j = 0; j < 10; j++){
      /*if(collision(helicopter[j].x,helicopter[j].x+200,raytracer[i].x,raytracer[i].x,helicopter[j].y,helicopter[j].y+40,raytracer[i].y,raytracer[i].y)){
        if(helicopter[j].on_screen){
          helicopter[j].health-=0.05;
          helicopter[j].hurt_timer=3;
        }
     }*/
    }
  }
}

//Bullet factory
void create_bullet( int newX, int newY, bool newOwner, float newAngle, float newSpeed){
  bullet newBullet;
  newBullet.x = newX;
  newBullet.y = newY;
  newBullet.vector_x = -newSpeed*cos( newAngle);
  newBullet.vector_y = -newSpeed*sin( newAngle);
  newBullet.owner = newOwner;
  bullets.push_back( newBullet);
  bullet_delay=0;
}

//Box factory
void create_box( int newX, int newY, int newType){
  bool box_made = false;
  for( int i = 0; i < 100; i++){
    if( !box[i].on_screen && !box_made){
      box_made = true;
      box[i].on_screen = true;
      box[i].x = newX;
      box[i].y = newY;
      box[i].type = newType;
    }
  }
  bullet_delay = 0;
}

//Finds angle of point 2 relative to point 1
float find_angle( int x_1, int y_1, int x_2, int y_2){
  float tan_1 = 0;
  float tan_2 = 0;
  if( x_1 - x_2 != 90 && y_1 - y_2 != 90 && x_1 - x_2 != 270 && y_1 - y_2 != 270){
    tan_1 = y_1 - y_2;
    tan_2 = x_1 - x_2;
  }
  return atan2(tan_1,tan_2);
}

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
  player_laser_timer--;
  bullet_delay++;

  // Reset timers
  if( player_fire_rate_timer < 1){
    player_fire_rate=5;
    player_fire_delay_rate=1;
  }

  // Set shooting angle
  mouse_angle_radians = find_angle( player_x + 15, player_y + 20, mouse_x, mouse_y);
  mouse_angle_allegro = mouse_angle_radians * 40.5845104792;

  // Shoot
  if(( key[KEY_SPACE] || mouse_b & 2) && bullet_delay > player_fire_delay_rate ){
    if( player_laser_timer < 1){
      create_bullet( player_x + 15, player_y + 20, PLAYER, mouse_angle_radians, player_fire_rate);
    }
    else{
      raytrace();
    }
  }

  // Update bullets
  for(int i = 0; i < bullets.size(); i++){
    bullets.at(i).x += bullets.at(i).vector_x;
    bullets.at(i).y += bullets.at(i).vector_y;
    if(collision( player_x, player_x + 50, bullets.at(i).x, bullets.at(i).x+5, player_y, player_y+50, bullets.at(i).y, bullets.at(i).y+5) && !bullets.at(i).owner){
      player_hurt_timer = 3;
      bullets.erase(bullets.begin() + i);
      player_health -= 5;
    }
    if( bullets.at(i).x > 800 || bullets.at(i).x < 0){
      bullets.at(i).vector_x = -bullets.at(i).vector_x;
    }
    if( bullets.at(i).y > 600 || bullets.at(i).y < 0){
      bullets.at(i).vector_y = -bullets.at(i).vector_y;
    }
    if( key[KEY_C]){
      bullets.clear();
    }
  }

  // Update boxes
  for(int i=0; i<10; i++){
    if(box[i].on_screen){
      if(box[i].y<550){
        box[i].y+=5;
      }
      if(collision(player_x,player_x+50,box[i].x,box[i].x+75,player_y,player_y+50,box[i].y,box[i].y+50)){
        box[i].on_screen=false;
        if(box[i].type==2){
          player_laser_timer=120;
        }
        if(box[i].type==1){
          if(player_health<90){
            player_health+=10;
          }
          else{
            player_health=100;
          }
        }
        if(box[i].type==0){
          player_fire_rate=20;
          player_fire_delay_rate=3;
          player_fire_rate_timer=600;
        }
      }
    }
  }
}

void draw(){
  // Draw background
  draw_sprite( buffer, background, 0, 0);

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
  for(int i=0; i < bullets.size(); i++){
    if(bullets.at(i).on_screen){
      if(bullets.at(i).owner){
        rectfill( buffer, bullets.at(i).x, bullets.at(i).y, bullets.at(i).x + 5, bullets.at(i).y + 5, makecol(0,0,0));
        rectfill( buffer, bullets.at(i).x + 1, bullets.at(i).y + 1, bullets.at(i).x + 4, bullets.at(i).y + 4, makecol(0,0,0));
        rectfill( buffer, bullets.at(i).x + 2, bullets.at(i).y + 2, bullets.at(i).x + 3, bullets.at(i).y + 3, makecol(0,0,0));
      }
      else{
        rectfill( buffer, bullets.at(i).x, bullets.at(i).y, bullets.at(i).x + 5, bullets.at(i).y + 5, makecol(255,0,0));
        rectfill( buffer, bullets.at(i).x + 1, bullets.at(i).y + 1, bullets.at(i).x + 4, bullets.at(i).y + 4, makecol(255,0,0));
        rectfill( buffer, bullets.at(i).x + 2, bullets.at(i).y + 2, bullets.at(i).x + 3, bullets.at(i).y + 3, makecol(255,0,0));
      }
    }
  }
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
  if (!(player = load_bitmap( "player.png", NULL)))
    abort_on_error( "Cannot find image player.png\nPlease check your files and try again");

  if (!(player_hurt = load_bitmap( "player_hurt.png", NULL)))
    abort_on_error( "Cannot find image player_hurt.png\nPlease check your files and try again");

  if (!(player_top = load_bitmap( "player_top.png", NULL)))
    abort_on_error( "Cannot find image player_top.png\nPlease check your files and try again");

  if (!(background = load_bitmap( "background.png", NULL)))
    abort_on_error( "Cannot find image background.png\nPlease check your files and try again");

  if (!(cursor = load_bitmap( "cursor.png", NULL)))
    abort_on_error( "Cannot find image cursor.png\nPlease check your files and try again");\

  if (!(bullet_image = load_bitmap( "bullet_image.png", NULL)))
    abort_on_error( "Cannot find image bullet_image.png\nPlease check your files and try again");

  if (!(helicopter_image = load_bitmap( "helicopter.png", NULL)))
    abort_on_error( "Cannot find image helicopter.png\nPlease check your files and try again");

  if (!(helicopter_hurt = load_bitmap( "helicopter_hurt.png", NULL)))
    abort_on_error( "Cannot find image helicopter_hurt.png\nPlease check your files and try again");
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
