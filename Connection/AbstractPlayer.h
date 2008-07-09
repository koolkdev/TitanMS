#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <string>
using namespace std;

class PacketHandler;
class AbstractPlayer {
public:
	virtual void setPacketHandler (PacketHandler* packetHandler) {
		this->packetHandler = packetHandler;
	}
	virtual void handleRequest (unsigned char* buf, int len) = 0;
	virtual ~AbstractPlayer(){}
	virtual string getIP(){
		return ip;
	}
	virtual void setIP(string ip){
		this->ip = ip;
	}
protected:
	PacketHandler* packetHandler;
	string ip;
};

class AbstractPlayerFactory {
public:
	virtual AbstractPlayer* createPlayer (int port) = 0;
};
#endif