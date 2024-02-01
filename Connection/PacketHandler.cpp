#include "PacketHandler.h"
//#include "Decoder.h"
#include <Winsock2.h>

#define HEADER_LEN 4
#define BUFFER_LEN 10000

PacketHandler::PacketHandler(int socket, AbstractPlayer* player) {
	this->socket = socket;
	buffer = new unsigned char[BUFFER_LEN];
	bytesInBuffer = 0;
	this->player = player;
	decoder = new Decoder();
	int l = send(socket, (char*)(decoder->getConnectPacket()), Decoder::CONNECT_LENGTH, 0);
	if (l < Decoder::CONNECT_LENGTH) {
		//TODO
	}
}

void PacketHandler::handle (Selector* selector, int socket) {
	if (bytesInBuffer < HEADER_LEN) {
		// read header
		int l = recv(socket, (char*)(buffer + bytesInBuffer), HEADER_LEN - bytesInBuffer, 0);
		if (l <= 0) {
			selector->unregisterSocket(socket);
			closesocket(socket);
			delete player;
		}
		bytesInBuffer += l;
	}

	if (bytesInBuffer >= HEADER_LEN) {
		int packetSize = Decoder::getLength(buffer);
		int l = recv(socket, (char*)(buffer + bytesInBuffer), HEADER_LEN + packetSize - bytesInBuffer, 0);
		if (l <= 0) {
			selector->unregisterSocket(socket);
			closesocket(socket);
			delete player;
		}
		bytesInBuffer += l;
		if (bytesInBuffer == packetSize + HEADER_LEN){
			decoder->decrypt(buffer + HEADER_LEN, packetSize);
			player->handleRequest(buffer + HEADER_LEN, packetSize);
			bytesInBuffer = 0;
		}
	}

}


void PacketHandler::sendPacket(unsigned char *buff, int size){
	unsigned char bufs[BUFFER_LEN];
	decoder->createHeader((unsigned char*)bufs, (short)(size));
	decoder->encrypt(buff, size);
	memcpy_s(bufs+4, size, buff, size);
	/*int count = 0;
	while(count < size){
		if(size+4-count > 1452){
			decoder->encrypt(buff+count, 1452);
			memcpy_s(bufs+count+4, 1452, buff+count, 1452);
		}
		else{
			decoder->encrypt(buff+count, size-count);
			memcpy_s(bufs+count+4, size-count, buff+count, size-count);
		}
		count += 1452;
	}*/
	decoder->next();
	send(socket, (const char*)bufs, size+4, 0);
}