#include "ServerNetwork.h"


ServerNetwork::ServerNetwork(void)
{
	//create WSADATA object
	WSADATA wsaData;

	//our sockets for the server
	m_listenSocket = INVALID_SOCKET;
	m_clientSocket = INVALID_SOCKET;

	//address info for the server to listen to
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error {%d}\n", iResult);
		exit(1);
	}

	//set address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; //TCP Connection
	hints.ai_flags = AI_PASSIVE;

	//THE SERVER WILL NOT NEED AN ADDRESS SINCE IT WILL BE ON LOCAL MACHIN//

	//Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error {%d}\n", iResult);
		WSACleanup();
		exit(1);
	}

	//Create a SOCKET for connecting to server
	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: {%ld}\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	//Set the mode of the socket to be non-blocking
	u_long iMode = 1;

	iResult = ioctlsocket(m_listenSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		WSACleanup();
		exit(1);
	}

	//Setup the TCP listneing socket
	iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		printf("bind failed with error: {%d}\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_listenSocket);
		WSACleanup();
		exit(1);
	}

	//no longer need address information
	freeaddrinfo(result);

	//start listening for new clients attempting to connect
	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: {%d}\n", WSAGetLastError());
		closesocket(m_listenSocket);
		WSACleanup();
		exit(1);
	}
}


ServerNetwork::~ServerNetwork(void)
{
}


//accept new connections
bool ServerNetwork::acceptNewClient(unsigned int &id)
{
	//if client waiting, accept the connection and save the socket
	m_clientSocket = accept(m_listenSocket, NULL, NULL);

	if (m_clientSocket != INVALID_SOCKET)
	{
		//disable nagle on the client's socket
		char value = 1;
		setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

		//insert new client into session id table
		sessions.insert(std::pair<unsigned int, SOCKET>(id, m_clientSocket));

		return true;
	}

	return false;
}

//receive incoming data
int ServerNetwork::receiveData(unsigned int client_id, char *recvbuff)
{
	if (sessions.find(client_id) != sessions.end())
	{
		SOCKET currentSocket = sessions[client_id];
		
		iResult = NetworkServices::receiveMessage(currentSocket, recvbuff, MAX_PACKET_SIZE);
		if (iResult == 0)
		{
			printf("Connection closed\n");
			closesocket(currentSocket);
		}
		return iResult;
	}
	return 0;
}


//send data to all clients
void ServerNetwork::sendToAll(char *packets, int totalSize)
{
	SOCKET currentSocket;
	
	std::map<unsigned int, SOCKET>::iterator iter;

	int iSendResult;

	for (iter = sessions.begin(); iter != sessions.end(); iter++)
	{
		currentSocket = iter->second;
		iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: {%d}\n", WSAGetLastError());
			closesocket(currentSocket);
		}
	}
}