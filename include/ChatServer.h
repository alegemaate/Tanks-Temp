#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <map>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

class World;

class ChatServer : private ISteamNetworkingSocketsCallbacks {
  public:
    void Open( uint16 nPort );

    void Run();

    void Close();

    World *GetWorld();

  private:

    HSteamListenSocket m_hListenSock;
    ISteamNetworkingSockets *m_pInterface;

    struct Client_t
    {
      int m_sID;
    };

    std::map<HSteamNetConnection, Client_t> m_mapClients;

    void SendStringToClient(HSteamNetConnection conn, const char *str);

    void SendStringToAllClients(const char *str, HSteamNetConnection except = k_HSteamNetConnection_Invalid);

    void PollIncomingMessages();

    void PollConnectionStateChanges();

    void PollLocalUserInput();

    virtual void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pInfo) override;

    int id_index;

    // World
    World *game_world;

    void SendMap(SteamNetConnectionStatusChangedCallback_t *pInfo);
};

#endif // CHATSERVER_H
