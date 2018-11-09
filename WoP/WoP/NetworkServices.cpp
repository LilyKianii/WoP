#include "NetworkServices.h"


NetworkServices::NetworkServices()
{
}


NetworkServices::~NetworkServices()
{
}

int NetworkServices::sendMessage(SOCKET curSocket, char* message, int messageSize)
{
	return send(curSocket, message, messageSize, 0);
}

int NetworkServices::receiveMessage(SOCKET curSocket, char* buffer, int buffSize)
{
	return recv(curSocket, buffer, buffSize, 0);
}