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
/*
#include "LoginWorlds.h"
#include "LoginPacket.h"
#include <string.h>
#include "PlayerLogin.h"
#include "Characters.h"
#include "ChannelsTemp.h"

void Worlds::showWorld(PlayerLogin* player){
	if(player->getStatus() != 4){
		// hacking
		return;
	}
	World world;
	strcpy_s(world.name, 15, "Scania");
	world.channels = ChannelsTemp::getChannels();;
	world.id = 0;
	LoginPacket::showWorld(player, world);
	LoginPacket::worldEnd(player);
}

void Worlds::selectWorld(PlayerLogin* player, unsigned char* packet){
	player->setServer(packet[0]);
	LoginPacket::showChannels(player);
}

void Worlds::channelSelect(PlayerLogin* player, unsigned char* packet){
	player->setChannel(packet[1]); 
	if(ChannelsTemp::getChannelStatus(player->getChannel()) != NULL){
		LoginPacket::channelSelect(player);
		Characters::showCharacters(player);
	}
	else{
		LoginPacket::loginError(player, 6);
	}
}*/