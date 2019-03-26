#include "tank.h"

#include "World.h"

unsigned char tank::num_bullet_bounces = 0;
BITMAP* tank::images[8] = { nullptr };
SAMPLE* tank::sample_shot = nullptr;

/*****************
  General Tank
*****************/
tank::tank(World *wrld, float x, float y, int type) :
  Entity(wrld, x, y)
{
  health = 100;
  initialHealth = health;
  fire_speed = 4;
  fire_delay_rate = 20;
  max_speed = 1;

  speed = 0;

  // Map size
  map_width = SCREEN_W;
  map_height = SCREEN_H;

  rotation_radians_body = 0;
  rotation_allegro_body = 0;
  rotation_radians_turret = 0;
  rotation_allegro_turret = 0;

  SetupTank(type);
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

// Explode
void tank::explode( int newX, int newY, int newVelocity, int newAmount, int newLife){
  for( int i = 0; i < newAmount; i ++){
    wrld -> AddParticle(new Particle(newX, newY, makecol(255,random(0,255),0), -newVelocity, newVelocity, -newVelocity, newVelocity, 1, CIRCLE, newLife, EXPLODE));
  }
}

// Move around
void tank::drive(float newRotation){
  canMoveX = true;
  canMoveY = true;

  if(canMoveX){
    vector_x = -speed * cos( newRotation);
    position.x += vector_x;
  }
  if(canMoveY){
    vector_y = -speed * sin( newRotation);
    position.y += vector_y;
  }
}

// Shoot
void tank::shoot(float rotation, float x, float y) {
  if( bullet_delay > fire_delay_rate ){
    bool magicMODE = key[KEY_LSHIFT];
    wrld -> AddEntity(new Bullet(wrld, x, y, rotation, fire_speed, team, 1 + num_bullet_bounces + (magicMODE * 10), sample_shot));
    bullet_delay = 0;
  }
}

// Update
void tank::Update() {

}

// Draw tank
void tank::drawTankBase( BITMAP* tempImage){
  // Hurt image for player
  if( health <= 0){
    rotate_sprite(tempImage, image_hurt, GetX(), GetY(), itofix(rotation_allegro_body));
  }
  else{
    rotate_sprite( tempImage, image_base, GetX(), GetY(), itofix(rotation_allegro_body));
  }
}

// Draw turret
void tank::drawTankTurret( BITMAP* tempImage){
  // Turret
  rotate_sprite( tempImage, image_top, GetX(), GetY(), itofix(rotation_allegro_turret));
}

// Draw health
void tank::drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight, int newBorderWidth){
  // Health Bar
  rectfill( tempImage, newX, newY, newX + newWidth, newY + newHeight, makecol(0,0,0));
  rectfill( tempImage, newX + newBorderWidth, newY + newBorderWidth,
           newX + newWidth - newBorderWidth, newY + newHeight - newBorderWidth, makecol(255,0,0));
  rectfill( tempImage, newX + newBorderWidth ,newY + newBorderWidth,
           newX + (((float)health/(float)initialHealth) * newWidth) - newBorderWidth, newY + newHeight - newBorderWidth,makecol(0,255,0));
}

// Draw
void tank::Draw( BITMAP* tempImage){
  // Tank
  drawTankBase( tempImage);

  // Turret
  drawTankTurret( tempImage);

  // Health bar
  if( health < initialHealth)
    drawHealthBar( tempImage, GetX() - 5, GetY() - 10, 50, 6, 1);
}

// Put decals
void tank::putDecal( BITMAP* tempImage){
  if(speed > 0)
    rotate_sprite( tempImage, image_treads, GetX() + GetWidth()/2, GetY(), itofix(rotation_allegro_body));
  /*else if( dead)
    drawTankBase( tempImage);*/
}


// Collide
void tank::Collide(Entity *other) {
  // Powerup collide
  if (Powerup* pPowerup = dynamic_cast<Powerup*>(other)) {
    get_powerup(pPowerup -> GetType());
    wrld -> RemoveEntity(other);
  }

  // Bullet collide
  if (Bullet* pBullet = dynamic_cast<Bullet*>(other)) {
    if (pBullet -> GetOwner() != team) {
      health -= 5;
      wrld -> RemoveEntity(other);
    }
  }
}

// Powerups
void tank::get_powerup(int powerup_id){
  if (powerup_id == 0) {
    health += 10;
    if(health > 100)
      health = 100;
  }
  else if (powerup_id == 1) {
    max_speed += 0.5;
  }
  else if (powerup_id == 2) {
    fire_speed += 1;
  }
  else if (powerup_id == 3) {
    fire_delay_rate -= 1;
    if (fire_delay_rate < 0)
      fire_delay_rate = 0;
  }
}


/*****************
   Player Tank
*****************/
// Init
player_tank::player_tank(World *wrld, int x, int y, int type) :
      tank(wrld, x, y, type){

}

// Update
void player_tank::Update(){
  // Shoot
  rotation_radians_turret = find_angle( SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + 25, y + 25, mouse_x, mouse_y);
  if(joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0)
    rotation_radians_turret = find_angle(GetX() + GetWidth()/2 - 2, GetY() + GetHeight()/2 - 2, (joy[0].stick[0].axis[0].pos) + (GetX() + 25), (joy[0].stick[0].axis[1].pos) + (GetY() + 25));
  rotation_allegro_turret = rotation_radians_turret * 40.5845104792;


  if(key[KEY_SPACE] || mouse_b & 1 || joy[0].button[1].b){
    shoot(rotation_radians_turret, GetX() + GetWidth()/2 - 2, GetY() + GetHeight()/2 - 2);
  }

  // Rotate with keys
  if(key[KEY_A] || key[KEY_LEFT]){
    rotation_radians_body -= 0.03;
    rotation_allegro_body = rotation_radians_body * 40.5845104792;
  }
  if(key[KEY_D] || key[KEY_RIGHT]){
    rotation_radians_body += 0.03;
    rotation_allegro_body = rotation_radians_body * 40.5845104792;
  }

  // Drive
  drive(rotation_radians_body);

  if(mouse_b & 2 || joy[0].button[0].b || key[KEY_W] || key[KEY_UP]){
    if(mouse_b & 2){
      rotation_radians_body = find_angle(SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + width/2, y + height/2, mouse_x, mouse_y);
    }
    else if(joy[0].button[0].b){
      rotation_radians_body = find_angle(GetX() + GetWidth()/2, GetY() + GetHeight()/2, (joy[0].stick[0].axis[0].pos) + (GetX() + GetWidth()/2), (joy[0].stick[0].axis[1].pos) + (GetY() + GetHeight()/2));
    }

    rotation_allegro_body = rotation_radians_body * 40.5845104792;

    // Accelerate
    if(speed == 0)
      speed = 0.2;
    else if(speed < max_speed)
      speed *= (max_speed * 1.03);
    else
      speed = max_speed;
  }
  else {
    // Decelerate
    if(speed > 0.1)
      speed *= 0.95;
    else
      speed = 0;
  }
}

/*****************
    AI Tank
*****************/
// Init
ai_tank::ai_tank(World *wrld, int x, int y, int type) :
  tank(wrld, x, y, type){

  destination_x = x;
  destination_y = y;
}

// Update
void ai_tank::Update(){
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
    rotation_radians_turret = rotation_radians_body;
    rotation_allegro_turret = rotation_allegro_body;
  }*/

  // Path
  update_target();

  // Drive
  if(random(0,100)){
    rotation_radians_body = find_angle(GetX() + 25, GetY() + 25, destination_x, destination_y);
    rotation_allegro_body = rotation_radians_body * 40.5845104792;

    // Accelerate
    if( speed == 0)
      speed = 0.2;
    else if( speed < 1)
      speed *= 1.03;
    else
      speed = 1;

    drive( rotation_radians_body);
  }
  else{
    speed = 0;
  }
}

// Ai point choosing
void ai_tank::update_target() {
  if(find_distance(GetX() + 25, GetY() + 25, destination_x, destination_y) < 10 || (canMoveX == false && canMoveY == false)){
    destination_x = random( 0, map_width);
    destination_y = random( 0, map_height);
  }
}
