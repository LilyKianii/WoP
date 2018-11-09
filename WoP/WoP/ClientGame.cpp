#include "ClientGame.h"


ClientGame::ClientGame(void)
{
	network = new ClientNetwork();

	//send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->m_connectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}


void ClientGame::sendActionPackets()
{
	//send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->m_connectSocket, packet_data, packet_size);
}


void ClientGame::update()
{
	Packet packet;
	int data_length = network->receivePackets(m_networkData);
	if (data_length <= 0)
		//no data received
		return;

	int i = 0;
	while (i < (unsigned int)data_length)
	{
		packet.deserialize(&(m_networkData[i]));
		i += sizeof(packet);

		switch (packet.packet_type)
		{
		case ACTION_EVENT:
			printf("client received action event packet from server\n");
			sendActionPackets();
			break;

		default:
			printf("error in packet types\n");
			break;
		}
	}
}