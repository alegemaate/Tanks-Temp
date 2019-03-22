#include "globals.h"

bool bClient = false;
bool bServer = false;

ChatClient client;
ChatServer server;

const uint16 DEFAULT_SERVER_PORT = 27020;

int nPort = DEFAULT_SERVER_PORT;
SteamNetworkingIPAddr addrServer;
