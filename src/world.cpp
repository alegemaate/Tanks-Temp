#include "World.h"

#include <algorithm>

#include "Barrier.h"
#include "tank.h"
#include "Powerup.h"

unsigned char World::map_width = 10;
unsigned char World::map_height = 10;

unsigned char World::num_enemies = 5;
unsigned char World::num_friends = 5;

World::World() {
  // Create buffer
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  // Load images
  background = load_bitmap_ex("images/background.png");

  // Init map
  InitializeMap(map_width, map_height);

  // Nullify camera tank
  camera_tank = nullptr;
}

World::~World() {
  destroy_bitmap(decal_buffer);
  destroy_bitmap(map_buffer);
  destroy_bitmap(background);
}


void World::GenerateMap(int width, int height) {
  // Make a map
  for( unsigned char pass = 1; pass < 7; pass++){
    for( unsigned char i = 0; i < width; i++){
      for( unsigned char t = 0; t < height; t++){
        // Pass 1 (Edges)
        if(pass == 1){
          map_temp[i][t] = 0;
          if( i == 0 || t == 0 || i == width - 1 || t == height - 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 2
        else if( pass == 2){
          if( map_temp[i][t] == 0 &&
             (map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
                   map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
                   map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
                   map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
                   random( 0, 2) == 1)){
            map_temp[i][t] = 2;
          }
        }
        // Pass 3 (Filling)
        else if( pass == 3 || pass == 4){
          if( map_temp[i][t] == 0 &&
             ((map_temp[i - 1][t] != 0 && map_temp[i + 1][t] != 0) ||
             (map_temp[i][t - 1] != 0 && map_temp[i][t + 1] != 0))){
            map_temp[i][t] = 2;
          }
        }
        // Pass 5 (Boxes!)
        else if( pass == 5){
          if( map_temp[i][t] == 0 && random( 1, 20) == 1){
            map_temp[i][t] = 3;
          }
        }
        // Pass 6 (Find start locations)
        else if( pass == 6){
          if( map_temp[i][t] == 0){
            startLocations.push_back(vec2<int>(i * 40, t * 40));
          }
          else {
            AddEntity(new Barrier(this, i * 40, t * 40, map_temp[i][t] - 1));
          }
        }
      }
    }
  }
}

void World::SetupTanks() {
  // Player
  int randomStartLocation = random(0, startLocations.size() - 1);
  player_tank *newPlayer = new player_tank(this, startLocations.at(randomStartLocation).x, startLocations.at( randomStartLocation).y, TANK_PLAYER);
  newPlayer -> SetMapDimensions(map_width * 40, map_height * 40);
  AddEntity(newPlayer);

  camera_tank = newPlayer;

  // Enemies
  /*for( unsigned char i = 0; i < num_enemies; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank(this, startLocations.at(randomStartLocation).x, startLocations.at( randomStartLocation).y, TANK_ENEMY);
    newPlayer -> set_map_dimensions(map_width * 40, map_height * 40);
    AddEntity(newPlayer);
  }

  // Friends
  for( unsigned char i = 0; i < num_friends; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank(this, startLocations.at(randomStartLocation).x, startLocations.at( randomStartLocation).y, TANK_FRIEND);
    newPlayer -> set_map_dimensions(map_width * 40, map_height * 40);
    AddEntity(newPlayer);
  }*/
}

void World::InitializeMap(int width, int height) {
  decal_buffer = create_bitmap(width * 40, height * 40);
  clear_to_color(decal_buffer, 0xFF00FF);

  map_buffer = create_bitmap(width * 40, height * 40);
  clear_to_color(map_buffer, 0x000000);

  map_x = 0;
  map_y = 0;
}

void World::AddEntity(Entity *entity) {
  if (entity)
    entities.push_back(entity);
}

void World::RemoveEntity(Entity *entity) {
  if (entity)
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

// Adds particle to global particle handler
void World::AddParticle(Particle *particle) {
  if (particle)
    particles.push_back(particle);
}

// Get decal buffer
BITMAP* World::GetDecalBuffer() {
  return decal_buffer;
}

// Updates world
void World::Update() {
  // Update entities
  for (unsigned int i = 0; i < entities.size(); i++) {
    entities.at(i) -> Update();
  }

  // Update particles
  for (unsigned int i = 0; i < particles.size(); i++) {
    particles.at(i) -> Update();

    //Check death of particles
    if (particles.at(i) -> IsDead())
      particles.erase(particles.begin() + i);
  }

  // Do collisions
  for (unsigned int i = 0; i < entities.size(); i++) {
    for (unsigned int t = 0; t < entities.size(); t++) {
      if (i < entities.size() && t < entities.size()) {
        if (entities.at(i) != entities.at(t) &&
            collision(entities.at(i) -> GetPosition(),
                      entities.at(i) -> GetDimensions(),
                      entities.at(t) -> GetPosition(),
                      entities.at(t) -> GetDimensions())) {
          entities.at(i) -> Collide(entities.at(t));
        }
      }
    }
  }

  // Game over
  /*if( key[KEY_SPACE] && (player_tanks.size() == 0 || enemy_tanks.size() == 0)){
    set_next_state( STATE_MENU);
  }*/

  // Scroll map
  if (camera_tank) {
    map_x = camera_tank -> GetPositionCenter().x - buffer -> w / 2;
    map_y = camera_tank -> GetPositionCenter().y - buffer -> h / 2;
  }
}

// Draw world
void World::Draw(BITMAP *buffer) {
  // Blank map map_buffer
  rectfill(map_buffer, 0, 0, map_buffer -> w, map_buffer -> h, makecol(0, 88, 0));

  // Draw background
  draw_sprite(buffer, background, 0, 0);

  // Decal to buffer
  draw_sprite(map_buffer, decal_buffer, 0, 0);

  // Draw game entities
  for(unsigned int i = 0; i < entities.size(); i++) {
    entities.at(i) -> Draw(map_buffer);
  }

  // Draw particles
  for(unsigned int i = 0; i < particles.size(); i++) {
    particles.at(i) -> Draw(map_buffer);
  }

  // Draw collision boxes
  for (unsigned int i = 0; i < entities.size(); i++) {
    rect(map_buffer, entities.at(i) -> GetX(),
                     entities.at(i) -> GetY(),
                     entities.at(i) -> GetX() + entities.at(i) -> GetWidth(),
                     entities.at(i) -> GetY() + entities.at(i) -> GetHeight(),
                     makecol(255,0,0));
  }

  // Map to buffer
  blit(map_buffer, buffer, map_x, map_y, 0, 0, buffer -> w, buffer -> h);
}
