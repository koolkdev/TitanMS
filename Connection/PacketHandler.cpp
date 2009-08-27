/*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "PacketHandler.h"
#include <Winsock2.h>
#include "../Decoder/Decoder.h"
#include "../Decoder/MasterDecoder.h"
#include "../Decoder/MapleEncryption.h"

#define HEADER_LEN 4
#define MASTER_HEADER_LEN 2

PacketHandlerMaple::PacketHandlerMaple(int socket, AbstractPlayer* player) {
	this->socket = socket;
	bytesInBuffer = 0;
	this->player = player;
	decoder = new Decoder();
	int l = send(socket, (char*)(decoder->getConnectPacket()), Decoder::CONNECT_LENGTH, 0);
	if (l < Decoder::CONNECT_LENGTH) {
		//TODO
	}
}
PacketHandlerMaple::~PacketHandlerMaple(){
	delete decoder;
	delete player;
}

int PacketHandlerMaple::handle (Selector* selector, int socket) {
	if (bytesInBuffer < HEADER_LEN) {
		// read header
		int l = recv(socket, (char*)(buffer + bytesInBuffer), HEADER_LEN - bytesInBuffer, 0);
		if (l <= 0) {
			end();
			return 0;
		}
		bytesInBuffer += l;
	}

	if (bytesInBuffer >= HEADER_LEN) {
		int packetSize = Decoder::getLength(buffer);
		int l = recv(socket, (char*)(buffer + bytesInBuffer), HEADER_LEN + packetSize - bytesInBuffer, 0);
		if (l <= 0) {
			end();
			return 0;
		}
		bytesInBuffer += l;
		if (bytesInBuffer == packetSize + HEADER_LEN){
			decoder->decrypt(buffer + HEADER_LEN, packetSize);
			player->handleRequest(buffer + HEADER_LEN, packetSize);
			bytesInBuffer = 0;
		}
	}
	return 1;

}


void PacketHandlerMaple::sendPacket(unsigned char *buff, int size){
	unsigned char bufs[BUFFER_LEN];
	decoder->createHeader((unsigned char*)bufs, (short)(size));
	decoder->encrypt(buff, size);
	memcpy_s(bufs+HEADER_LEN, size, buff, size);
	decoder->next();
	send(socket, (const char*)bufs, size+HEADER_LEN, 0);
}
PacketHandlerMaster::PacketHandlerMaster(int socket, AbstractPlayer* player) {
	this->socket = socket;
	bytesInBuffer = 0;
	this->player = player;
	decoder = new MasterDecoder();
}
PacketHandlerMaster::~PacketHandlerMaster(){
	delete decoder;
	delete player;
}

int PacketHandlerMaster::handle (Selector* selector, int socket) {
	if (bytesInBuffer < MASTER_HEADER_LEN) {
		// read header
		int l = recv(socket, (char*)(buffer + bytesInBuffer), MASTER_HEADER_LEN - bytesInBuffer, 0);
		if (l <= 0) {
			end();
			return 0;
		}
		bytesInBuffer += l;
	}

	if (bytesInBuffer >= MASTER_HEADER_LEN) {
		int packetSize = MasterDecoder::getLength(buffer);
		int l = recv(socket, (char*)(buffer + bytesInBuffer), MASTER_HEADER_LEN + packetSize - bytesInBuffer, 0);
		if (l <= 0) {
			end();
			return 0;
		}
		bytesInBuffer += l;
		if (bytesInBuffer == packetSize + MASTER_HEADER_LEN){
			decoder->decrypt(buffer + MASTER_HEADER_LEN, packetSize);
			player->handleRequest(buffer + MASTER_HEADER_LEN, packetSize);
			bytesInBuffer = 0;
		}
	}
	return 1;
}


void PacketHandlerMaster::sendPacket(unsigned char *buff, int size){
	unsigned char bufs[BUFFER_LEN];
	decoder->createHeader((unsigned char*)bufs, (short)(size));
	decoder->encrypt(buff, size);
	memcpy_s(bufs+MASTER_HEADER_LEN, size, buff, size);
	send(socket, (const char*)bufs, size+MASTER_HEADER_LEN, 0);
}
void PacketHandlerMaple::disconnect(){
	end();
}
void PacketHandlerMaster::disconnect(){
	end();
}