#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#define BUFFER_LEN 10000

#include "../Decoder/Decoder.h"
#include "../Decoder/MasterDecoder.h"
#include "../Decoder/MapleEncryption.h"
#include "Selector.h"
#include "AbstractPlayer.h"

class PacketHandler: public Selector::SelectHandler {
public:

	virtual int handle (Selector* selector, int socket) = 0;
	virtual void sendPacket(unsigned char* buf, int len) = 0;
	virtual ~PacketHandler(){}
	virtual void disconnect() = 0;
};

class PacketHandlerMaple: public PacketHandler {
public:

	PacketHandlerMaple(int socket, AbstractPlayer* player);
	~PacketHandlerMaple();
	int handle (Selector* selector, int socket);
	void sendPacket(unsigned char* buf, int len);

private:

	unsigned char buffer[BUFFER_LEN];
	int bytesInBuffer;
	AbstractPlayer* player;
	Decoder* decoder;
	int socket;
	void disconnect();
};

class PacketHandlerMaster: public PacketHandler {
public:

	PacketHandlerMaster(int socket, AbstractPlayer* player);
	~PacketHandlerMaster();
	int handle (Selector* selector, int socket);
	void sendPacket(unsigned char* buf, int len);
	void disconnect();

private:

	unsigned char buffer[BUFFER_LEN];
	int bytesInBuffer;
	AbstractPlayer* player;
	int socket;
	MasterDecoder* decoder;
};

#endif