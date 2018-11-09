#include "ClientNetwork.h"


ClientNetwork::ClientNetwork(void)
{
	//create WSADATA object
	WSADATA wsaData;

	//socket
	m_connectSocket = INVALID_SOCKET;

	//holds address info for socket to connect to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error {%d}\n", iResult);
		exit(1);
	}

	//set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; //TCP connection

	//resolve server address and port
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error {%d}\n", iResult);
		WSACleanup();
		exit(1);
	}

	//Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		//create a SOCKET for connection to server
		m_connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (m_connectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error {%d}\n", iResult);
			WSACleanup();
			exit(1);
		}

		//Connect to server
		iResult = connect(m_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(m_connectSocket);
			m_connectSocket = INVALID_SOCKET;
			printf("Server is down... Did not connect");
		}
	}

	//no longuer need address info for server
	freeaddrinfo(result);

	//if connection failed
	if (m_connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}

	//Set the mode of the socket to non-blocking
	u_long iMode = 1;

	iResult = ioctlsocket(m_connectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error {%d}\n", WSAGetLastError());
		closesocket(m_connectSocket);
		WSACleanup();
		exit(1);
	}

	//disable nagle
	char value = 1;
	setsockopt(m_connectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}


ClientNetwork::~ClientNetwork(void)
{
}


int ClientNetwork::receivePackets(char *recvbuff)
{
	iResult = NetworkServices::receiveMessage(m_connectSocket, recvbuff, MAX_PACKET_SIZE);
	if (iResult == 0)
	{
		printf("Connection closed\n");
		closesocket(m_connectSocket);
		WSACleanup();
		exit(1);
	}

	return iResult;
}
