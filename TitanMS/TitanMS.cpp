#include "MySQLM.h"
#include "../Connection/Acceptor.h"
#include "../Connection/Connector.h"
#include "../Decoder/Decoder.h"
#include "PlayerLogin.h"
//#include "Player.h"
#include <stdlib.h>
#include "Initializing.h"
#include "Timer.h"
#include "Skills.h"
#include "Maps.h"
#include "Server.h"
#include "Worlds.h"
#include "AngelScriptEngine.h"
#include "Tools.h"
#include "Run.h"
#include "mysql.h"
using namespace Tools;


void main(){ 
	int Version = 55;
	printf("Initializing Angel Script Engine... ");
	if(AngelScriptEngine::Initialize())
		printf("DONE\n");
	else{
		printf("Failed to create Angel Script engine.\n");
		exit(1);
	}
	Initializing::initializing(); 
	printf("Initializing Timers... ");
	class RunTest : public Run{
		void run(){
			printf("TEST");
		}
	};
	Timer::getInstance()->setTimer(500, new RunTest());
	//Skills::startTimer();
	//Maps::startTimer();
	printf("DONE\n");
	printf("Initializing Decoder... ");
	Decoder::Initialize(Version);
	printf("DONE\n");
	printf("Initializing MySQL... ");
	if(MySQL::getInstance()->connectToMySQL())
		printf("DONE\n");
	else{
		printf("FAILED\n");
		exit(1);
	}
	//Server::initialize();
	randomize();
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)  printf("Error at WSAStartup()\n");

	srand((unsigned char)time(0));

	Selector* selector = new Selector();
	new Acceptor(8484, selector, new PlayerLoginFactory());
	new Worlds(selector, 2, 2);
	while(getchar() != 'x'){}
}