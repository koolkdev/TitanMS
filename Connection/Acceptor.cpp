#include <stdio.h>
#include <Winsock2.h>
#include "Acceptor.h"
#include "PacketHandler.h"
hash_map <int, int> Acceptor::ports;

Acceptor::Acceptor(int port, Selector* selector, AbstractPlayerFactory* apf, bool master) {
	abstractPlayerFactory = apf;

	SOCKET acceptSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (acceptSocket == INVALID_SOCKET) {
		printf ("socket error: %d\n", WSAGetLastError());
		return;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	service.sin_port = htons(port);

	if (bind( acceptSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR) {
		printf("bind() error: %d\n", WSAGetLastError());
		return;
	}

	if (listen( acceptSocket, 10 ) == SOCKET_ERROR) {
		printf("listen() error: %d\n", WSAGetLastError());
		closesocket(acceptSocket);
		return;
	}

	this->master = master;

	selector->registerSocket (acceptSocket, true, false, true, this);

	ports[acceptSocket] = port;

}

int Acceptor::handle (Selector* selector, int socket) {	
	sockaddr_in cli_addr;
	char IP[50];
	int cli_len = sizeof(cli_addr);
	SOCKET sock = accept( socket, (struct sockaddr *) &cli_addr,&cli_len);
	//SOCKET sock = accept( socket, NULL, NULL );
	printf ("accept\n");
	if (sock == INVALID_SOCKET) {
		printf("accept error: %d\n", WSAGetLastError());
		return 0;
	}
	sprintf_s(IP,20,"%d.%d.%d.%d",cli_addr.sin_addr.S_un.S_un_b.s_b1,cli_addr.sin_addr.S_un.S_un_b.s_b2,cli_addr.sin_addr.S_un.S_un_b.s_b3,cli_addr.sin_addr.S_un.S_un_b.s_b4);

	AbstractPlayer* player = abstractPlayerFactory->createPlayer(ports[socket]);
	player->setIP(string(IP));
	PacketHandler* ph;
	if(master)
		ph = new PacketHandlerMaster(sock, player);
	else
		ph = new PacketHandlerMaple(sock, player);
	player->setPacketHandler(ph);
	selector->registerSocket (sock, true, false, true, ph);
	return 1;
}
