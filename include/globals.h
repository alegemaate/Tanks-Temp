#include "ChatCommon.h"
#include "ChatServer.h"
#include "ChatClient.h"

class ChatClient;
class ChatServer;

extern bool bClient;
extern bool bServer;

extern ChatClient client;
extern ChatServer server;

extern const uint16 DEFAULT_SERVER_PORT;

extern int nPort;
extern SteamNetworkingIPAddr addrServer;
