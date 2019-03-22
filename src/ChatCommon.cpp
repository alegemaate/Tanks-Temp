#include "ChatCommon.h"

#include <string>
#include <thread>
#include <chrono>

#define WIN32

#ifdef WIN32
	#include <windows.h> // Ug, for NukeProcess -- see below
#else
	#include <unistd.h>
	#include <signal.h>
#endif

bool g_bQuit = false;

SteamNetworkingMicroseconds g_logTimeZero;

ChatCommon cc;

void DebugOutput( ESteamNetworkingSocketsDebugOutputType eType, const char *pszMsg ) {
	SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - g_logTimeZero;
	printf( "%10.6f %s\n", time*1e-6, pszMsg );
	fflush(stdout);
	if ( eType == k_ESteamNetworkingSocketsDebugOutputType_Bug )
	{
		fflush(stdout);
		fflush(stderr);
		exit(1);
	}
}

void ChatCommon::FatalError( const char *fmt, ... ) {
	char text[ 2048 ];
	va_list ap;
	va_start( ap, fmt );
	vsprintf( text, fmt, ap );
	va_end(ap);
	char *nl = strchr( text, '\0' ) - 1;
	if ( nl >= text && *nl == '\n' )
		*nl = '\0';
	DebugOutput( k_ESteamNetworkingSocketsDebugOutputType_Bug, text );
}

void ChatCommon::Printf( const char *fmt, ... ) {
	char text[ 2048 ];
	va_list ap;
	va_start( ap, fmt );
	vsprintf( text, fmt, ap );
	va_end(ap);
	char *nl = strchr( text, '\0' ) - 1;
	if ( nl >= text && *nl == '\n' )
		*nl = '\0';
	DebugOutput( k_ESteamNetworkingSocketsDebugOutputType_Msg, text );
}

void ChatCommon::InitSteamDatagramConnectionSockets() {
  SteamDatagramErrMsg errMsg;
  if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
    cc.FatalError( "GameNetworkingSockets_Init failed.  %s", errMsg );

	g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

	SteamNetworkingUtils()->SetDebugOutputFunction( k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput );
}

void ChatCommon::ShutdownSteamDatagramConnectionSockets() {
	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

  GameNetworkingSockets_Kill();
}


