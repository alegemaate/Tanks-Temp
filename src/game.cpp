#include "game.h"

#include "globals.h"
#include "World.h"

// Init state (and game)
game::game(){
  // Multiplayer
  if (bServer) {
    addrServer.Clear();
    server.Open((uint16)nPort);
  }

  if (bClient) {
    addrServer.Clear();
    addrServer.ParseString("127.0.0.1");
    addrServer.m_port = DEFAULT_SERVER_PORT;
    client.Open(addrServer);
  }

  // Create buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);
  cursor = load_bitmap_ex( "images/cursor.png");
}

// Clean up
game::~game(){
  destroy_bitmap(buffer);
  destroy_bitmap(cursor);


  if (bClient) {
		client.Close();
		bClient = false;
	}
	if (bServer) {
		server.Close();
		bServer = false;
	}
}


void game::update(){
  // Networking
  if (bClient) {
		client.Run();
	}
	if (bServer) {
		server.Run();
	}

  // Get joystick input
  poll_joystick();
}


void game::draw(){
  // Draw world
  if (bClient && client.GetWorld()) {
    client.GetWorld() -> Draw(buffer);
  }
  else if (bServer && server.GetWorld()) {
    server.GetWorld() -> Draw(buffer);
  }

  // Text
  /*textprintf_ex( buffer, font, 20, 20, makecol(0,0,0), makecol(255,255,255), "Round: %i", currentRound);
  textprintf_ex( buffer, font, 20, 35, makecol(0,0,0), makecol(255,255,255), "Team BLUE: %i", player_tanks.size());
  textprintf_ex( buffer, font, 20, 50, makecol(0,0,0), makecol(255,255,255), "Team RED: %i", enemy_tanks.size());*/

  // Cursor
  draw_sprite(buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite(screen, buffer, 0, 0);
}
