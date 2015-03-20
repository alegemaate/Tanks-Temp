#include "../include/tank.h"

/*****************
  General Tank
*****************/
tank::tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage)
{
  x = newX;
  y = newY;

  hurt_timer = newHurtTime;
  health = newHealth;
  initialHealth = newHealth;
  fire_speed = newFireSpeed;

  fire_delay_rate = newFireDelay;
  speed = newSpeed;

  image_base = newBaseImage;
  image_hurt = newHurtImage;
  image_top = newTurretImage;

  dead = false;
  pendingErase = false;
}

// Check dead
bool tank::isDead(){
  // Just died
  if( !dead && (health < 1))
    explode( x + 25, y + 25, 20, 2000, 200);

  // Set dead
  dead = (health < 1);

  return dead;
}

// Check erase
bool tank::getErase(){
  return pendingErase;
}

// Explode
void tank::explode( int newX, int newY, int newVelocity, int newAmount, int newLife){
  for( int i = 0; i < newAmount; i ++){
    particle newParticle(newX, newY, makecol(255,random(0,255),0), -newVelocity, newVelocity, -newVelocity, newVelocity, 1, CIRCLE, newLife, EXPLODE);
    explosionEffect.push_back(newParticle);
  }
}

// Get bullets
vector<bullet>* tank::getBullets(){
  return &bullets;
}

// Move around
void tank::drive( float newRotation){
  vector_x = -speed * cos( newRotation);
  vector_y = -speed * sin( newRotation);
  x += vector_x;
  y += vector_y;
}

// Update timers
void tank::update_timers(){
  // Change timers
  hurt_timer--;
  bullet_delay++;
}

// Update bullets
void tank::update_bullets(){
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
}

// Shoot
void tank::shoot( float newRotation, float newX, float newY){
  if( bullet_delay > fire_delay_rate ){
    // Load sounds
    SAMPLE* fire;
    if (!(fire = load_sample( "sfx/fire.wav")))
      abort_on_error( "Cannot find image sfx/fire.wav\nPlease check your files and try again");
    bullet newBullet( newX, newY, newRotation, fire_speed, true, 4, fire);
    bullets.push_back( newBullet);
    bullet_delay = 0;
  }
}

// Update
void tank::update(){
}

// Draw bullets
void tank::drawBullets( BITMAP* tempImage){
  // Draw bullets
  for( unsigned int i = 0; i < bullets.size(); i++)
    bullets.at(i).draw( tempImage);
}

// Draw tank
void tank::drawTankBase( BITMAP* tempImage){
  // Hurt image for player
  if( hurt_timer < 1){
    rotate_sprite( tempImage, image_base, x, y, itofix(rotation_allegro_body));
  }
  else{
    rotate_sprite( tempImage, image_hurt, x, y, itofix(rotation_allegro_body));
  }
}

// Draw turret
void tank::drawTankTurret( BITMAP* tempImage){
  // Turret
  rotate_sprite( tempImage, image_top, x, y, itofix(rotation_allegro_turret));
}

// Draw health
void tank::drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight){
  // Health Bar
  rectfill(tempImage,newX,newY,newX + newWidth,newY + newHeight,makecol(0,0,0));
  rectfill(tempImage,newX+2,newY+2,newX + newWidth - 2,newY + newHeight-2,makecol(255,0,0));
  rectfill(tempImage,newX+2,newY+2,newX + ((health/initialHealth) * newWidth) - 2,newY + newHeight-2,makecol(0,255,0));
}

// Draw
void tank::draw( BITMAP* tempImage){
  // Tank
  drawTankBase( tempImage);

  // Bullets
  drawBullets( tempImage);

  // Turret
  if( !isDead()){
    drawTankTurret( tempImage);

    // Health bar
    drawHealthBar( tempImage, x - 20, y - 20, 90, 10);
  }
  else{
    for( unsigned int i = 0; i < explosionEffect.size(); i++){
      explosionEffect.at(i).draw(tempImage);
    }
  }
}


/*****************
   Player Tank
*****************/
// Init
player_tank::player_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage) :
      tank( newX, newY, newHurtTime, newHealth, newFireSpeed, newFireDelay, newSpeed, newBaseImage, newTurretImage, newHurtImage){

}

// Update
void player_tank::update(){
  if( !isDead()){
    // Shoot
    rotation_radians_turret = find_angle( x + 25, y + 25, mouse_x, mouse_y);
    rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

    if( key[KEY_SPACE] || mouse_b & 1){
      shoot( rotation_radians_turret, x + 23, y + 23);
    }

    // Drive
    if( mouse_b & 2){
      rotation_radians_body = find_angle( x + 25, y + 25, mouse_x, mouse_y);
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
      drive( rotation_radians_body);
      speed = 1;
    }
    else{
      speed = 0;
    }
  }
  else{
    // Update particles
    for( unsigned int i = 0; i < explosionEffect.size(); i++){
      explosionEffect.at(i).logic();
      //Check death of particles
      if(explosionEffect.at(i).getDead())
        explosionEffect.erase(explosionEffect.begin() + i);
    }

    // Delete bullet once particles are all dead
    if( explosionEffect.size() <= 0)
      pendingErase = true;
  }

  // Update bullets
  update_timers();
  update_bullets();
}

/*****************
    AI Tank
*****************/
// Init
ai_tank::ai_tank( int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage) :
      tank( newX, newY, newHurtTime, newHealth, newFireSpeed, newFireDelay, newSpeed, newBaseImage, newTurretImage, newHurtImage){
  destination_x = x;
  destination_y = y;
}

// Update
void ai_tank::update(){
  if( !isDead()){
    // Rotate turret
    rotation_radians_turret += float(random(-1,1)/10);
    rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

    // Shoot
    if( random(0,100) == 0){
      shoot( rotation_radians_turret, x + 23, y + 23);
    }

    // Path
    update_target();

    // Drive
    if( random(0,100)){
      rotation_radians_body = find_angle( x + 25, y + 25, destination_x, destination_y);
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
      speed = 1;
      drive( rotation_radians_body);
    }
    else{
      speed = 0;
    }
  }
  else{
    // Update particles
    for( unsigned int i = 0; i < explosionEffect.size(); i++){
      explosionEffect.at(i).logic();
      //Check death of particles
      if(explosionEffect.at(i).getDead())
        explosionEffect.erase(explosionEffect.begin() + i);
    }

    // Delete bullet once particles are all dead
    if( explosionEffect.size() <= 0)
      pendingErase = true;
  }

  // Update bullets
  update_timers();
  update_bullets();
}

// Ai point choosing
void ai_tank::update_target(){
  if( find_distance(x + 25, y + 25, destination_x, destination_y) < 10){
    destination_x = random( 0, 800);
    destination_y = random( 0, 600);
  }
}
