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

#ifndef PLAYERLOGINHANDLER_H
#define PLAYERLOGINHANDLER_H

class PacketReader;
class PlayerLogin;
#include "Handler.h"

class PlayerLoginHandler : public Handler<PlayerLoginHandler>{
private:
	static const unsigned short PING = 0x19;
	static const unsigned short LOGIN_REQUEST = 0x1;
	static const unsigned short CHANNEL_SELECT = 0x5;
	static const unsigned short WORLD_SELECT = 0x6;
	static const unsigned short HANDLE_LOGIN = 0x09;
	static const unsigned short SHOW_WORLD = 0xB;
	static const unsigned short WORLD_BACK = 0x4;
	static const unsigned short CHARACTER_SELECT = 0x13;
	static const unsigned short NAME_CHECK = 0x15;
	static const unsigned short CREATE_CHARACTER = 0x16;
	static const unsigned short DELETE_CHARACTER = 0x17;
	static const unsigned short LOGIN_BACK = 0x1C;
	//static const unsigned short SET_GENDER = 0x07;
	//static const unsigned short REGISTER_PIN = 0x09;


	PlayerLogin* player;
public:
	PlayerLoginHandler(PlayerLogin* player){
		this->player = player;
	}
	static void loadPackets();
	void pingHandle(PacketReader& packet);
	void loginRequestHandle(PacketReader& packet);
	void channelSelectHandle(PacketReader& packet);
	void worldSelectHandle(PacketReader& packet);
	void loginHandle(PacketReader& packet);
	void showWorldRequestHandle(PacketReader& packet);
	void characterSelectHandle(PacketReader& packet);
	void nameSelectHandle(PacketReader& packet);
	void createCharacterHandle(PacketReader& packet);
	void deleteCharacterHandle(PacketReader& packet);
	void loginBackHandle(PacketReader& packet);
	//void setGenderHandle(PacketReader& packet);
	//void registerPINHandle(PacketReader& packet);
};

#endif