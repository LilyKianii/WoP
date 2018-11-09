#include <WinSock2.h>
#include <Windows.h>
#include "ClientNetwork.h"

class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	void sendActionPackets();

	void update();


	ClientNetwork* network;
	char m_networkData[MAX_PACKET_SIZE];

	

	
};

