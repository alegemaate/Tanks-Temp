#include "game.h"

// Init state (and game)
game::game(){
  // Create buffer
  buffer = create_bitmap( 800, 600);
  decal_buffer = create_bitmap( map_width * 40, map_height * 40);
  rectfill( decal_buffer, 0, 0, map_width * 40, map_height * 400, makecol( 255, 0, 255));

  vision_buffer = create_bitmap( 800, 600);
  rectfill( vision_buffer, 0, 0, 800, 600, makecol( 0, 0, 0));

  map_buffer = create_bitmap( map_width * 40, map_height * 40);
  rectfill( vision_buffer, 0, 0, map_width * 40, map_height * 40, makecol( 0, 0, 0));

  // Load images
  background = load_bitmap_ex( "images/background.png");
  cursor = load_bitmap_ex( "images/cursor.png");
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
      if( map_temp[i][t] == 1 || map_temp[i][t] == 2){
        barrier newBarrier( i * 40, t * 40, blocks[map_temp[i][t]], -1);

        // Destroyable
        if( map_temp[i][t] == 2)
          newBarrier.setHealth(3);

        barriers.push_back( newBarrier);
      }
    }
  }

  // Player
  int randomStartLocation = random( 0, startLocations.size() - 1);
  player_tank *newPlayer = new player_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                          100, 4, 20, 1,
                          tank_images[3], tank_images[2], tank_images[1], tank_images[0]);

  newPlayer -> process_enemies( &enemy_tanks);
  newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
  player_tanks.push_back( newPlayer);

  // Enemies
  for( int i = 0; i < num_enemies; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                      random(50,150), random(1,4), random(50,300), random(1,10)/10,
                      tank_images[5], tank_images[4], tank_images[1], tank_images[0]);

    newPlayer -> process_enemies( &player_tanks);
    newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
    enemy_tanks.push_back( newPlayer);
  }

  // Friends
  for( int i = 0; i < num_friends; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank( startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                          100, 4, 20, 1,
                          tank_images[7], tank_images[6], tank_images[1], tank_images[0]);

    newPlayer -> process_enemies( &enemy_tanks);
    newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
    player_tanks.push_back( newPlayer);
  }
}


void game::update(){
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

    // Collision with powerups
    enemy_tanks.at(i) -> checkCollision( &powerups);

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

    // Collision with powerups
    player_tanks.at(i) -> checkCollision( &powerups);

    // Update tanks
    player_tanks.at(i) -> update();

    // Delete tank
    if( player_tanks.at(i) -> getErase()){
      delete player_tanks[i];
      player_tanks.erase(player_tanks.begin() + i);
    }
  }

  // Remove broken barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    if( barriers.at(i).getDead()){
      // Spawn powerup
      if( random( 0, 1) == 0){
        int type = random( 0, 3);
        powerup newPowerup( barriers.at(i).getX(), barriers.at(i).getY(), type, powerup_images[type]);
        powerups.push_back( newPowerup);
      }

      barriers.erase( barriers.begin() + i);
    }
  }

  // Delete powerup
  for( unsigned int i = 0; i < powerups.size(); i++){
    if(powerups.at(i).getDead()){
      powerups.erase(powerups.begin() + i);
    }
  }

  // Game over
  if( player_tanks.size() == 0 || enemy_tanks.size() == 0){
    set_next_state( STATE_MENU);
  }

  // Scroll map
  if( player_tanks.size() > 0){
    map_x = player_tanks.at(0) -> getX() + player_tanks.at(0) -> getWidth()/2 - buffer -> w / 2;
    map_y = player_tanks.at(0) -> getY() + player_tanks.at(0) -> getHeight()/2 - buffer -> h / 2;
  }
}


void game::draw(){
// Draw background
  draw_sprite( buffer, background, 0, 0);

  // Blank map map_buffer
  rectfill( map_buffer, 0, 0, map_buffer -> w, map_buffer -> h, makecol( 0, 88, 0));

  // Decal to buffer
  draw_sprite( map_buffer, decal_buffer, 0, 0);

  // Draw tanks
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    enemy_tanks.at(i) -> draw( map_buffer);
    enemy_tanks.at(i) -> putDecal( decal_buffer);
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    player_tanks.at(i) -> draw( map_buffer);
    player_tanks.at(i) -> putDecal( decal_buffer);
  }

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++)
    barriers.at(i).draw( map_buffer);

  // Draw powerups
  for( unsigned int i = 0; i < powerups.size(); i++){
    powerups.at(i).draw( map_buffer);
  }

  // Map to buffer
  blit( map_buffer, buffer, map_x, map_y, 0, 0, buffer -> w, buffer -> h);

  // Text
  textprintf_ex( buffer, font, 20, 20, makecol(0,0,0), makecol(255,255,255), "Round: %i", currentRound);
  textprintf_ex( buffer, font, 20, 35, makecol(0,0,0), makecol(255,255,255), "Team BLUE: %i", player_tanks.size());
  textprintf_ex( buffer, font, 20, 50, makecol(0,0,0), makecol(255,255,255), "Team RED: %i", enemy_tanks.size());

  // Cursor
  draw_sprite( buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}
