//Networking libraries
#include <WinSock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <WS2tcpip.h>
#include <stdio.h>

//Size of buffer
#define DEFAULT_BUFFLEN 512

//port to connect sockets through
#define DEFAULT_PORT "6881"

//Need to link with Ws2_32.lib, Mswsock.lib and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

class ClientNetwork
{
public:
	ClientNetwork(void);
	~ClientNetwork(void);

	int receivePackets(char *);

	//for error checking function calls in Winsock library
	int iResult; // --> not private ???

	//socket for client to connect to server
	SOCKET m_connectSocket;
};

