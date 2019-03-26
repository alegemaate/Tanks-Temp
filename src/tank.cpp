#include "tank.h"

#include "World.h"

#include <iostream>

unsigned char tank::num_bullet_bounces = 0;
BITMAP* tank::images[8] = { nullptr };
SAMPLE* tank::sample_shot = nullptr;

/*****************
  General Tank
*****************/
tank::tank(World *wrld, float x, float y, int type) :
  Entity(wrld, x, y) {
  health = 100;
  max_health = health;
  fire_speed = 4;
  fire_delay = 500;

  total_velocity = 0.0f;
  max_velocity = 1.0f;

  // Map size
  map_dimensions = vec2<int>(0, 0);

  rotation_body = 0;
  rotation_turret = 0;

  SetupTank(type);

  // Start bullet timer
  fire_timer.Start();
}

// Delete
tank::~tank(){

}

void tank::SetupTank(int type) {
  // Set team
  team = type;

  // Load sound
  if (sample_shot == nullptr) {
    sample_shot = load_sample_ex( "sfx/fire.wav");
  }

  // Load images
  if (images[0] == nullptr) {
    images[0] = load_bitmap_ex( "images/tank_treads.png");
    images[1] = load_bitmap_ex( "images/tank_dead.png");
    images[2] = load_bitmap_ex( "images/tank_turret_green.png");
    images[3] = load_bitmap_ex( "images/tank_base_green.png");
    images[4] = load_bitmap_ex( "images/tank_turret_red.png");
    images[5] = load_bitmap_ex( "images/tank_base_red.png");
    images[6] = load_bitmap_ex( "images/tank_turret_blue.png");
    images[7] = load_bitmap_ex( "images/tank_base_blue.png");
  }

  // Choose skin
  switch (type) {
    case TANK_PLAYER:
    {
      image_base = images[3];
      image_top = images[2];
      image_hurt = images[1];
      image_treads = images[0];
      break;
    }
    case TANK_FRIEND:
    {
      image_base = images[5];
      image_top = images[4];
      image_hurt = images[1];
      image_treads = images[0];
      break;
    }
    case TANK_ENEMY:
    {
      image_base = images[7];
      image_top = images[6];
      image_hurt = images[1];
      image_treads = images[0];
      break;
    }
  }

  // Set dimensions
  SetDimensions(vec2<int>(image_base -> w, image_base -> h));
}

// Move around
void tank::Drive(float rotation) {
  velocity.x = -total_velocity * cos(rotation);
  velocity.y = -total_velocity * sin(rotation);
  if (canMoveX) {
    position.x += velocity.x;
  }
  if (canMoveY) {
    position.y += velocity.y;
  }
}

// Shoot
void tank::Shoot(float rotation, float x, float y) {
  if (fire_timer.GetElapsedTime<std::chrono::milliseconds>() > fire_delay) {
    wrld -> AddEntity(new Bullet(wrld, x, y, rotation, fire_speed, team, 1 + num_bullet_bounces, sample_shot));
    fire_timer.Reset();
  }
}

// Destroy
void tank::Destroy(int velocity, int amount, int life) {
  // Create EXPLOSION
  for(int i = 0; i < amount; i++) {
    wrld -> AddParticle(new Particle(GetX(), GetY(), makecol(255,random(0,255),0), -velocity, velocity, -velocity, velocity, 1, CIRCLE, life, EXPLODE));
  }

  // Remove from world
  wrld -> RemoveEntity(this);
}

// Update
void tank::Update() {

}

// Draw tank
void tank::DrawBase(BITMAP* buffer) {
  // Hurt image for player
  if (health <= 0) {
    rotate_sprite(buffer, image_hurt, GetX(), GetY(), itofix(rotation_body * RAD_TO_FIX));
  }
  else {
    rotate_sprite(buffer, image_base, GetX(), GetY(), itofix(rotation_body * RAD_TO_FIX));
  }
}

// Draw turret
void tank::DrawTurret(BITMAP* buffer) {
  // Turret
  rotate_sprite(buffer, image_top, GetX(), GetY(), itofix(rotation_turret * RAD_TO_FIX));
}

// Draw health
void tank::DrawHealthBar(BITMAP* buffer, int x, int y, int width, int height, int border_width) {
  // Health Bar
  rectfill(buffer, x, y, x + width, y + height, makecol(0,0,0));
  rectfill(buffer, x + border_width, y + border_width,
           x + width - border_width, y + height - border_width, makecol(255,0,0));
  rectfill(buffer, x + border_width ,y + border_width,
           x + (((float)health/(float)max_health) * width) - border_width, y + height - border_width, makecol(0,255,0));
}

// Draw
void tank::Draw(BITMAP* buffer) {
  // Add decal
  DrawDecal();

  // Tank
  DrawBase(buffer);

  // Turret
  DrawTurret(buffer);

  // Health bar
  if (health < max_health)
    DrawHealthBar(buffer, GetX() - 5, GetY() - 10, 50, 6, 1);
}

// Put decals
void tank::DrawDecal() {
  if (total_velocity > 0)
    rotate_sprite(wrld -> GetDecalBuffer(), image_treads, GetX() + GetWidth()/2, GetY(), itofix(rotation_body * RAD_TO_FIX));
  else if (health <= 0)
    DrawBase(wrld -> GetDecalBuffer());
}


// Collide
void tank::Collide(Entity *other) {
  // Powerup collide
  if (Powerup* pPowerup = dynamic_cast<Powerup*>(other)) {
    PickupPowerup(pPowerup -> GetType());
    wrld -> RemoveEntity(other);
  }

  // Bullet collide
  else if (Bullet* pBullet = dynamic_cast<Bullet*>(other)) {
    if (pBullet -> GetOwner() != team) {
      health -= 5;
      wrld -> RemoveEntity(other);
    }
  }

  // Barrier collide
  else if (Barrier* pBarrier = dynamic_cast<Barrier*>(other)) {
    if (GetX() + GetWidth() + velocity.x > other -> GetX() &&
        GetX() + velocity.x < other -> GetX() &&
        velocity.x > 0.0f) {
      std::cout << "TRIGGER x 1" << std::endl;
      position.x = other -> GetX() - GetWidth();
    }
    if (GetX() + velocity.x < other -> GetX() + other -> GetWidth() &&
        GetX() + velocity.x > other -> GetX() &&
        velocity.x < 0.0f) {
      std::cout << "TRIGGER x 2" << std::endl;
      position.x = other -> GetX() + other -> GetWidth();
    }
    if (GetY() + GetHeight() + velocity.y > other -> GetY() &&
        GetY() + velocity.y < other -> GetY() &&
        velocity.y > 0.0f) {
      std::cout << "TRIGGER y 1" << std::endl;
      position.y = other -> GetY() - GetHeight();
    }
    if (GetY() + velocity.y < other -> GetY() + other -> GetHeight() &&
        GetY() + velocity.y > other -> GetY() &&
        velocity.y < 0.0f) {
      std::cout << "TRIGGER y 2" << std::endl;
      position.y = other -> GetY() + other -> GetHeight();
    }
  }
}

// Powerups
void tank::PickupPowerup(int powerup_id) {
  if (powerup_id == 0) {
    health += 10;
    if(health > 100)
      health = 100;
  }
  else if (powerup_id == 1) {
    max_velocity += 0.5;
  }
  else if (powerup_id == 2) {
    fire_speed += 1;
  }
  else if (powerup_id == 3) {
    if (fire_delay > 0)
      fire_delay -= 50;
  }
}


/*****************
   Player Tank
*****************/
// Init
player_tank::player_tank(World *wrld, int x, int y, int type) :
      tank(wrld, x, y, type) {

}

// Update
void player_tank::Update() {
  // Shoot
  rotation_turret = find_angle( SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + 25, y + 25, mouse_x, mouse_y);
  if (joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0)
    rotation_turret = find_angle(GetX() + GetWidth()/2 - 2, GetY() + GetHeight()/2 - 2, (joy[0].stick[0].axis[0].pos) + (GetX() + 25), (joy[0].stick[0].axis[1].pos) + (GetY() + 25));

  if (key[KEY_SPACE] || mouse_b & 1 || joy[0].button[1].b)
    Shoot(rotation_turret, GetX() + GetWidth()/2 - 2, GetY() + GetHeight()/2 - 2);

  // Rotate with keys
  if (key[KEY_A] || key[KEY_LEFT])
    rotation_body -= 0.03;
  if (key[KEY_D] || key[KEY_RIGHT])
    rotation_body += 0.03;

  // Drive
  Drive(rotation_body);

  if (mouse_b & 2 || joy[0].button[0].b || key[KEY_W] || key[KEY_UP]) {
    if (mouse_b & 2) {
      rotation_body = find_angle(SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + width/2, y + height/2, mouse_x, mouse_y);
    }
    else if (joy[0].button[0].b) {
      rotation_body = find_angle(GetX() + GetWidth()/2, GetY() + GetHeight()/2, (joy[0].stick[0].axis[0].pos) + (GetX() + GetWidth()/2), (joy[0].stick[0].axis[1].pos) + (GetY() + GetHeight()/2));
    }

    // Accelerate
    if (total_velocity < 0.01f)
      total_velocity = 0.2f;
    else if (total_velocity < max_velocity)
      total_velocity *= (max_velocity * 1.03f);
    else
      total_velocity = max_velocity;
  }
  else {
    // Decelerate
    if (total_velocity > 0.1f)
      total_velocity *= 0.95f;
    else
      total_velocity = 0.0f;
  }

  // Assume can move next turn
  canMoveX = true;
  canMoveY = true;
}

/*****************
    AI Tank
*****************/
// Init
ai_tank::ai_tank(World *wrld, int x, int y, int type) :
  tank(wrld, x, y, type) {

  destination_x = x;
  destination_y = y;
}

// Update
void ai_tank::Update() {
  // Rotate turret (at closest enemy)
  int best_enemy_x = destination_x;
  int best_enemy_y = destination_y;
  int target_enemy_index = 0;

  // Find nearest enemy and assign to target_enemy_index
  /*if( otherTanks -> size() > 0){
    int temp_enemy_x, temp_enemy_y;

    for( unsigned int i = 0; i < otherTanks -> size(); i++){
      temp_enemy_x = otherTanks -> at(i) -> getX() + otherTanks -> at(i) -> getWidth()/2;
      temp_enemy_y = otherTanks -> at(i) -> getY() + otherTanks -> at(i) -> getHeight()/2;

      best_enemy_x = otherTanks -> at(target_enemy_index) -> getX() + otherTanks -> at(target_enemy_index) -> getWidth()/2;
      best_enemy_y = otherTanks -> at(target_enemy_index) -> getY() + otherTanks -> at(target_enemy_index) -> getHeight()/2;

      if( find_distance( x, y, temp_enemy_x, temp_enemy_y) <
          find_distance( x, y, best_enemy_x, best_enemy_y)){
        target_enemy_index = i;
        best_enemy_x = otherTanks -> at(target_enemy_index) -> getX() + otherTanks -> at(target_enemy_index) -> getWidth()/2;
        best_enemy_y = otherTanks -> at(target_enemy_index) -> getY() + otherTanks -> at(target_enemy_index) -> getHeight()/2;
      }
    }

    rotation_radians_turret = find_angle( x + 25, y + 25, best_enemy_x, best_enemy_y);//randomf(-0.1,0.1);
    rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

    // Shoot
    if( random(0,10) == 0 && find_distance( x, y, best_enemy_x, best_enemy_y) < 500){
      shoot( rotation_radians_turret, x + 23, y + 23);
    }
  }
  else{
    rotation_radians_turret = rotation_body;
    rotation_allegro_turret = rotation_body;
  }*/

  // Path
  update_target();

  // Drive
  if (random(0,100)) {
    rotation_body = find_angle(GetX() + 25, GetY() + 25, destination_x, destination_y);

    // Accelerate
    if (total_velocity == 0)
      total_velocity = 0.2;
    else if (total_velocity < max_velocity)
      total_velocity *= 1.03;
    else
      total_velocity = max_velocity;

    Drive(rotation_body);
  }
  else {
    total_velocity = 0.0f;
  }
}

// Ai point choosing
void ai_tank::update_target() {
  if (find_distance(GetX() + 25, GetY() + 25, destination_x, destination_y) < 10 || (canMoveX == false && canMoveY == false)) {
    destination_x = random(0, map_dimensions.x);
    destination_y = random(0, map_dimensions.y);
  }
}
