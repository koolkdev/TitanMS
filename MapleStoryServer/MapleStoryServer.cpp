#include "../Connection/Acceptor.h"
#include "PlayerLogin.h"
#include "Player.h"
#include "MySQLM.h"
#include <stdlib.h>
#include "Initializing.h"
#include "Timer.h"
#include "Skills.h"

void main(){

	Initializing::initializing();
	Timer::timer = new Timer();
	Skills::startTimer();
	MySQL::connectToMySQL();
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)  printf("Error at WSAStartup()\n");

	srand((unsigned char)time(0));

	Selector* selector = new Selector();
	Acceptor::Acceptor(8484, selector, new PlayerLoginFactory());
	for(int i=0; i<5; i++){
		Acceptor::Acceptor(8888+i, selector, new PlayerFactory());
	}
	while(getchar()){}
}