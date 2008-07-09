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
#include "Party.h"
#include "PartyPacket.h"
#include "PacketCreator.h"
#include "Player.h"

void PartyPacket::createParty(Player* player){
	Packet packet = Packet();
	packet.addHeader(0x39);
	packet.addByte(8);
	packet.addShort(0x8b);
	packet.addShort(2);
	packet.addBytes("ffc99a3bffc99a3b");
	packet.packetSend(player);
}
void PartyPacket::inviteParty(Player* player, Player* in){
	Packet packet = Packet();
	packet.addHeader(0x39);
	packet.addByte(4);
	packet.addInt(in->getParty()->getPartyID());
	packet.addString(player->getName());
	packet.packetSend(player);
}
void PartyPacket::partyError(Player* player, int error){
	Packet packet = Packet();
	packet.addHeader(0x39);
	packet.addInt(error);
	packet.packetSend(player);
}

void PartyPacket::partyReplay(Player* player, char* to){
	Packet packet = Packet();
	packet.addHeader(0x39);
	packet.addInt(22);
	packet.addShort(strlen(to));
	packet.addString(to);
	packet.packetSend(player);
}*/