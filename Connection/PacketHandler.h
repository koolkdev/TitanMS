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

#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#define BUFFER_LEN 32000

class Decoder;
class MasterDecoder;
#include "Selector.h"
#include "AbstractPlayer.h"

class PacketHandler: public Selector::SelectHandler {
public:

	virtual int handle (Selector* selector, int socket) = 0;
	virtual void sendPacket(unsigned char* buf, int len){} // temp(?) fix to prevent crashes(probably because the handler is already deleted, multi-thread problem?), should be = 0;
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