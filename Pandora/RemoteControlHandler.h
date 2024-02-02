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

#ifndef REMOTEHANDLER_H
#define REMOTEHANDLER_H

class PacketReader;
#include "Handler.h"
class RemoteControl;

class RemoteControlHandler : public Handler<RemoteControlHandler>{
private:
	static const short LOGIN_FAIL = -1;
	static const short LOGIN = 0x00;
	static const short COMMAND = 0x01;

	RemoteControl* remote;
public:
	RemoteControlHandler(RemoteControl* remote){
		this->remote = remote;
	}
	static void loadPackets();
	void loginHandle(PacketReader& packet);
	void loginFailedHandle(PacketReader& packet);
	void commandHandle(PacketReader& packet);

};

#endif