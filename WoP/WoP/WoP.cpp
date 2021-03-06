// WoP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>

void serverLoop(void*);
void clientLoop(void);

ServerGame *server;
ClientGame *client;

void serverLoop(void *arg)
{
	while (true)
		server->update();
}

void clientLoop()
{
	while (true)
	{
		//do game stuff
		client->update();
	}
}

int main()
{
    //initialize the server
	server = new ServerGame();

	//RUNNING SERVER ON SEPARATE  THREAD FOR TESTING//

	//create thread with arbitrary argument for the run function
	_beginthread(serverLoop, 0, (void*)12);




	// initialize the client
	client = new ClientGame();

	clientLoop();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
