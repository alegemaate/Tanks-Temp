#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

class World;

class ChatClient : private ISteamNetworkingSocketsCallbacks {
  public:
    void Open( const SteamNetworkingIPAddr &serverAddr );

    void Run();

    void Close();

    World *GetWorld();

  private:

    HSteamNetConnection m_hConnection;
    ISteamNetworkingSockets *m_pInterface;

    void PollIncomingMessages();

    void PollConnectionStateChanges();

    void PollLocalUserInput();

    virtual void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pInfo) override;

    // World
    World *game_world;

};

#endif // CHATCLIENT_H
