#include "world.h"

unsigned char world::map_width = 10;
unsigned char world::map_height = 10;

unsigned char world::num_enemies = 5;
unsigned char world::num_friends = 5;

world::world() {
  // Create buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Load images
  background = load_bitmap_ex( "images/background.png");
  blocks[0] = load_bitmap_ex( "images/block_box_1.png");
  blocks[1] = load_bitmap_ex( "images/block_stone_1.png");
  blocks[2] = load_bitmap_ex( "images/block_box_1.png");
  powerup_images[0] = load_bitmap_ex( "images/powerup_health.png");
  powerup_images[1] = load_bitmap_ex( "images/powerup_tank_speed.png");
  powerup_images[2] = load_bitmap_ex( "images/powerup_bullet_speed.png");
  powerup_images[3] = load_bitmap_ex( "images/powerup_bullet_delay.png");
  tank_images[0] = load_bitmap_ex( "images/tank_treads.png");
  tank_images[1] = load_bitmap_ex( "images/tank_dead.png");
  tank_images[2] = load_bitmap_ex( "images/tank_turret_green.png");
  tank_images[3] = load_bitmap_ex( "images/tank_base_green.png");
  tank_images[4] = load_bitmap_ex( "images/tank_turret_red.png");
  tank_images[5] = load_bitmap_ex( "images/tank_base_red.png");
  tank_images[6] = load_bitmap_ex( "images/tank_turret_blue.png");
  tank_images[7] = load_bitmap_ex( "images/tank_base_blue.png");

  // Init map
  init_map(map_width, map_height);
}

world::~world() {
  destroy_bitmap(decal_buffer);
  destroy_bitmap(map_buffer);

  destroy_bitmap(background);

  destroy_bitmap(blocks[0]);
  destroy_bitmap(blocks[1]);
  destroy_bitmap(blocks[2]);

  for( int i = 0; i < 4; i++)
    destroy_bitmap(powerup_images[i]);

  for( int i = 0; i < 8; i++)
    destroy_bitmap(tank_images[i]);
}


void world::generate_map(int width, int height) {
  // Make a map
  for( unsigned char pass = 1; pass < 7; pass++){
    for( unsigned char i = 0; i < width; i++){
      for( unsigned char t = 0; t < height; t++){
        // Pass 1 (Edges)
        if( pass == 1){
          map_temp[i][t] = 0;
          if( i == 0 || t == 0 || i == width - 1 || t == height - 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 2
        else if( pass == 2){
          if( map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
                   map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
                   map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
                   map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
                   random( 0, 2) == 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 3 (Filling)
        else if( pass == 3){
          if( (map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1) ||
              (map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1)){
            map_temp[i][t] = 1;
          }
        }
        // Pass 4 (Filling inaccessable areas)
        else if( pass == 4){
          if( map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1 &&
              map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 5 (Boxes!)
        else if( pass == 5){
          if( map_temp[i][t] == 0 && random( 1, 20) == 1){
            map_temp[i][t] = 2;
          }
        }
        // Pass 6 (Find start locations)
        else if( pass == 6){
          if( map_temp[i][t] == 0){
            coordinate newStartLocation;
            newStartLocation.x = i * 40;
            newStartLocation.y = t * 40;
            startLocations.push_back( newStartLocation);
          }
          else {
            place_barrier(i * 40, t * 40, map_temp[i][t]);
          }
        }
      }
    }
  }
}

void world::init_map(int width, int height) {
  decal_buffer = create_bitmap( width * 40, height * 40);
  clear_to_color( decal_buffer, 0xFF00FF);

  map_buffer = create_bitmap( width * 40, height * 40);
  clear_to_color( map_buffer, 0x000000);

  map_x = 0;
  map_y = 0;
}

void world::place_barrier(int x, int y, int type) {
  if (type < 1 || type > 3)
    return;

  barrier* newBarrier = new barrier(this, x, y, blocks[type], 0);

  // Destroyable
  if( type == 2)
    newBarrier -> setHealth(3);
  else
    newBarrier -> setIndestructable(true);

  barriers.push_back(newBarrier);
}


// Adds particle to global particle handler
void world::addParticle(particle *newParticle) {
  particles.push_back(newParticle);
}

// Updates world
void world::update() {
  // Update particles
  for(unsigned int i = 0; i < particles.size(); i++) {
    particles.at(i) -> logic();

    //Check death of particles
    if(particles.at(i) -> getDead())
      particles.erase( particles.begin() + i);
  }

  // Remove broken barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    if( barriers.at(i) -> getDead()){
      // Spawn powerup
      /*if( random( 0, 1) == 0){
        int type = random( 0, 3);
        powerup *newPowerup = new powerup( barriers.at(i) -> getX(), barriers.at(i) -> getY(), type, powerup_images[type]);
        powerups.push_back( newPowerup);
      }*/

      barriers.erase( barriers.begin() + i);
    }
  }

  // Delete powerup
  /*for( unsigned int i = 0; i < powerups.size(); i++){
    if(powerups.at(i).getDead()){
      powerups.erase(powerups.begin() + i);
    }
  }*/

  // Game over
  /*if( key[KEY_SPACE] && (player_tanks.size() == 0 || enemy_tanks.size() == 0)){
    set_next_state( STATE_MENU);
  }*/

  // Scroll map
  /*if( player_tanks.size() > 0){
    map_x = player_tanks.at(0) -> getCenterX() - buffer -> w / 2;
    map_y = player_tanks.at(0) -> getCenterY() - buffer -> h / 2;
  }*/
}

// Draw world
void world::draw(BITMAP *buffer) {
  // Blank map map_buffer
  rectfill(map_buffer, 0, 0, map_buffer -> w, map_buffer -> h, makecol( 0, 88, 0));

  // Draw background
  draw_sprite(buffer, background, 0, 0);

  // Decal to buffer
  draw_sprite(map_buffer, decal_buffer, 0, 0);

  // Draw tanks
  /*for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    enemy_tanks.at(i) -> draw( map_buffer);
    enemy_tanks.at(i) -> putDecal( decal_buffer);
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    player_tanks.at(i) -> draw( map_buffer);
    player_tanks.at(i) -> putDecal( decal_buffer);
  }*/

  // Draw particles
  for( unsigned int i = 0; i < particles.size(); i++){
    particles.at(i) -> draw(map_buffer);
  }

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++)
    barriers.at(i) -> draw(map_buffer);

  // Draw powerups
  /*for( unsigned int i = 0; i < powerups.size(); i++)
    powerups.at(i).draw( map_buffer);*/

  // Map to buffer
  blit( map_buffer, buffer, map_x, map_y, 0, 0, buffer -> w, buffer -> h);
}

// Play 3d sample
void world::play_sample_2d(SAMPLE* spl, int vol, int pan, int freq, bool loop) {

}
