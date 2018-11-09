#include <WinSock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <WS2tcpip.h>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFFLEN 512
#define DEFAULT_PORT "6881"

class ServerNetwork
{
public:
	ServerNetwork(void);
	~ServerNetwork(void);

	//Socket to listen for new connections
	SOCKET m_listenSocket;

	//Socket to give to the clients
	SOCKET m_clientSocket;

	//for error checking return values
	int iResult;

	//table to keep track of each client's socket
	std::map<unsigned int, SOCKET> sessions;

	//accept new connections
	bool acceptNewClient(unsigned int &id);

	//receive incoming data
	int receiveData(unsigned int client_id, char *recvbuff);

	//send data to all clients
	void sendToAll(char *packets, int totalSize);
};

