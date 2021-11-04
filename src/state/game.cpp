#include "game.h"

#include <algorithm>

#include "../system/ImageRegistry.h"
#include "state-engine.hpp"

unsigned char Game::map_width = 20;
unsigned char Game::map_height = 20;

unsigned char Game::num_enemies = 5;
unsigned char Game::num_friends = 5;

// Init state (and Game)
Game::Game() {
  // Create buffers
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  decal_buffer = create_bitmap(map_width * 40, map_height * 40);
  clear_to_color(decal_buffer, 0xFF00FF);

  vision_buffer = create_bitmap(map_width * 40, map_height * 40);
  clear_to_color(vision_buffer, 0x000000);

  map_buffer = create_bitmap(map_width * 40, map_height * 40);
  clear_to_color(map_buffer, 0x000000);

  // Load images
  background = ImageRegistry::getImage("game-background");
  cursor = ImageRegistry::getImage("cursor");
  blocks[0] = ImageRegistry::getImage("block-box");
  blocks[1] = ImageRegistry::getImage("block-stone");
  blocks[2] = ImageRegistry::getImage("block-box");

  // Make a map
  for (unsigned char pass = 0; pass < 8; pass++) {
    for (unsigned char i = 0; i < map_width; i++) {
      for (unsigned char t = 0; t < map_height; t++) {
        // Pass 0 (Erase)
        if (pass == 0) {
          map_temp[i][t] = BarrierType::NONE;
        }
        // Pass 1 (Edges)
        else if (pass == 1) {
          if (i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 2 (Well Placed blocks)
        else if (pass == 2) {
          if (map_temp[i - 1][t] == BarrierType::NONE &&
              map_temp[i + 1][t] == BarrierType::NONE &&
              map_temp[i - 1][t + 1] == BarrierType::NONE &&
              map_temp[i + 1][t + 1] == BarrierType::NONE &&
              map_temp[i - 1][t - 1] == BarrierType::NONE &&
              map_temp[i + 1][t - 1] == BarrierType::NONE &&
              map_temp[i][t - 1] == BarrierType::NONE &&
              map_temp[i][t + 1] == BarrierType::NONE && random(0, 2) == 1) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 3 (Filling)
        else if (pass == 3) {
          if ((map_temp[i - 1][t] == BarrierType::STONE &&
               map_temp[i + 1][t] == BarrierType::STONE) ||
              (map_temp[i][t - 1] == BarrierType::STONE &&
               map_temp[i][t + 1] == BarrierType::STONE)) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 4 (Filling inaccessable areas)
        else if (pass == 4) {
          if (map_temp[i - 1][t] == BarrierType::STONE &&
              map_temp[i + 1][t] == BarrierType::STONE &&
              map_temp[i][t - 1] == BarrierType::STONE &&
              map_temp[i][t + 1] == BarrierType::STONE) {
            map_temp[i][t] = BarrierType::STONE;
          }
        }
        // Pass 5 (Boxes!)
        else if (pass == 5) {
          if (map_temp[i][t] == BarrierType::NONE && random(1, 20) == 1) {
            map_temp[i][t] = BarrierType::BOX;
          }
        }
        // Pass 6 (Find start locations)
        else if (pass == 6) {
          if (map_temp[i][t] == BarrierType::NONE) {
            startLocations.push_back(Coordinate(i * 40, t * 40));
          }
        }
        // Pass 7 (create barriers)
        else if (pass == 7 && map_temp[i][t] != BarrierType::NONE) {
          auto position = Coordinate(i * 40, t * 40);
          auto barrier = new Barrier(&game_world, position, map_temp[i][t]);

          if (i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1) {
            barrier->makeIndestructable(true);
          }

          barriers.push_back(barrier);
        }
      }
    }
  }

  // Player

  Coordinate startLocation =
      startLocations.at(random(0, startLocations.size() - 1));
  PlayerTank* player = new PlayerTank(&game_world, startLocation.x,
                                      startLocation.y, 100, 4, 20, 1);

  player->process_enemies(&enemy_tanks);
  player->set_map_dimensions(map_width * 40, map_height * 40);
  player_tanks.push_back(player);

  // Enemies
  for (unsigned char i = 0; i < num_enemies; i++) {
    startLocation = startLocations.at(random(0, startLocations.size() - 1));
    AiTank* player =
        new AiTank(&game_world, startLocation.x, startLocation.y,
                   random(50, 150), random(1, 8), random(50, 300), 1, true);

    player->process_enemies(&player_tanks);
    player->set_map_dimensions(map_width * 40, map_height * 40);
    enemy_tanks.push_back(player);
  }

  // Friends
  for (unsigned char i = 0; i < num_friends; i++) {
    startLocation = startLocations.at(random(0, startLocations.size() - 1));
    AiTank* player = new AiTank(&game_world, startLocation.x, startLocation.y,
                                100, 4, 150, 1, false);

    player->process_enemies(&enemy_tanks);
    player->set_map_dimensions(map_width * 40, map_height * 40);
    player_tanks.push_back(player);
  }
}

// Clean up
Game::~Game() {
  destroy_bitmap(buffer);
  destroy_bitmap(decal_buffer);
  destroy_bitmap(vision_buffer);
  destroy_bitmap(map_buffer);
}

void Game::update() {
  // Get joystick input
  poll_joystick();

  // Update world
  game_world.update();

  // Move
  for (auto const& enemy : enemy_tanks) {
    // Update barriers
    for (auto const& barrier : barriers) {
      barrier->update(enemy->getBullets());
    }

    // Update bullets
    for (auto const& player : player_tanks) {
      player->checkCollision(enemy->getBullets());
    }

    // Collision with barrier
    enemy->checkCollision(&barriers);

    // Collision with powerups
    enemy->checkCollision(&powerups);

    // Update tanks
    enemy->update();
  }

  for (auto const& player : player_tanks) {
    // Update barriers
    for (auto const& barrier : barriers) {
      barrier->update(player->getBullets());
    }

    // Update bullets
    for (auto const& enemy : enemy_tanks) {
      enemy->checkCollision(player->getBullets());
    }

    // Collision with barrier
    player->checkCollision(&barriers);

    // Collision with powerups
    player->checkCollision(&powerups);

    // Update tanks
    player->update();
  }

  // Remove broken barriers
  for (auto const& barrier : barriers) {
    if (barrier->getDead()) {
      // Spawn powerup
      if (random(0, 1) == 0) {
        int randomType = random(0, 3);
        PowerupType type = PowerupType::Health;

        switch (randomType) {
          case 1:
            type = PowerupType::Speed;
            break;
          case 2:
            type = PowerupType::FireSpeed;
            break;
          case 3:
            type = PowerupType::FireDelay;
            break;
          default:
          case 0:
            type = PowerupType::Health;
            break;
        }

        Powerup* powerup =
            new Powerup(barrier->position.x, barrier->position.y, type);
        powerups.push_back(powerup);
      }
    }
  }

  // Cleanup barriers
  barriers.erase(
      std::remove_if(barriers.begin(), barriers.end(),
                     [](auto const& barrier) { return barrier->getDead(); }),
      barriers.end());

  // Cleanup powerups
  powerups.erase(
      std::remove_if(powerups.begin(), powerups.end(),
                     [](auto const& powerup) { return powerup->getDead(); }),
      powerups.end());

  // Game over
  if (key[KEY_SPACE] && (player_tanks.size() == 0 || enemy_tanks.size() == 0)) {
    StateEngine::setNextState(StateId::STATE_MENU);
  }

  // Scroll map
  if (player_tanks.size() > 0) {
    map_x = player_tanks.at(0)->getCenterX() - buffer->w / 2;
    map_y = player_tanks.at(0)->getCenterY() - buffer->h / 2;
  }
}

void Game::draw() {
  // Draw background
  draw_sprite(buffer, background, 0, 0);

  // Blank map map_buffer
  rectfill(map_buffer, 0, 0, map_buffer->w, map_buffer->h, makecol(0, 88, 0));

  // Decal to buffer
  draw_sprite(map_buffer, decal_buffer, 0, 0);

  // Draw tanks
  for (auto const& enemy : enemy_tanks) {
    enemy->draw(map_buffer);
    enemy->putDecal(decal_buffer);
  }

  for (auto const& player : player_tanks) {
    player->draw(map_buffer);
    player->putDecal(decal_buffer);
  }

  // Draw world
  game_world.draw(map_buffer);

  // Draw barriers
  for (auto const& barrier : barriers) {
    barrier->draw(map_buffer);
  }

  // Draw powerups
  for (auto const& powerup : powerups) {
    powerup->draw(map_buffer);
  }

  // Map to buffer
  blit(map_buffer, buffer, map_x, map_y, 0, 0, buffer->w, buffer->h);

  // Text
  textprintf_ex(buffer, font, 20, 20, makecol(0, 0, 0), makecol(255, 255, 255),
                "Round: %i", currentRound);
  textprintf_ex(buffer, font, 20, 35, makecol(0, 0, 0), makecol(255, 255, 255),
                "Team BLUE: %i", player_tanks.size());
  textprintf_ex(buffer, font, 20, 50, makecol(0, 0, 0), makecol(255, 255, 255),
                "Team RED: %i", enemy_tanks.size());

  // Cursor
  draw_sprite(buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite(screen, buffer, 0, 0);
}
