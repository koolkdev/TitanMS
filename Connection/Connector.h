#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "../Decoder/Decoder.h"
#include "../Decoder/MasterDecoder.h"
#include <Winsock2.h>
#include <windows.h>

class Connector {
public:
	Connector (char* ip, int port);
	void handle();
	virtual void handlePacket(unsigned char* buf, int size) = 0;
	void sendPacket(unsigned char* buf, int size);
private:
	SOCKET sock;
	MasterDecoder*  decoder;
};

#endif