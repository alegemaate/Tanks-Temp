#ifndef CHATCOMMON_H
#define CHATCOMMON_H

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

extern bool g_bQuit;

extern SteamNetworkingMicroseconds g_logTimeZero;

void DebugOutput( ESteamNetworkingSocketsDebugOutputType eType, const char *pszMsg );

class ChatCommon {
  public:
    void FatalError( const char *fmt, ... );

    void Printf( const char *fmt, ... );

    void InitSteamDatagramConnectionSockets();

    void ShutdownSteamDatagramConnectionSockets();
};

extern ChatCommon cc;

#endif // CHATCOMMON_H
