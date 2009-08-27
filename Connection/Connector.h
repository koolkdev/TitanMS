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