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

#include "../Connection/PacketHandler.h"
#include "DataProvider.h"
#include "RemoteControl.h"
#include "RemoteControlHandler.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "PacketHandlingError.h"
#include "MySQLM.h"
#include <iostream>

RemoteControl::RemoteControl () {
	correct = false;
	handler = new RemoteControlHandler(this);
}
RemoteControl::~RemoteControl() {
	delete handler;
}
void RemoteControl::handleRequest(unsigned char* buf, int len){
	try{
		handler->handle(PacketReader(buf, len, -1));
	}
#ifdef _DEBUG
	catch(PacketHandlingError ph){
		cout << ph.getError();
	}
	catch (mysqlpp::BadQuery er ) {
		printf("%s\n", er.what());
	}
#endif
	catch(...){
		//TODO
	}
}

void RemoteControl::send(PacketWriter* packet){
	unsigned char bytes [MAX_LENGTH];
	memcpy_s(bytes, packet->getLength(), packet->getBytes(), packet->getLength());
	sendPacket(bytes, packet->getLength());
}