 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#include "DropsPacket.h"
#include "PacketCreator.h"
#include "Player.h"
#include "Drops.h"

void DropsPacket::drop(vector <Player*> players, Drop* drop, Dropped dropper){
	Packet packet = Packet();
	packet.addHeader(0xB9);
	packet.addByte(1);
	packet.addInt(drop->getObjID());
	packet.addByte(drop->getMesos());
	packet.addInt(drop->getID());
	packet.addInt(dropper.id);
	packet.addByte(0);
	packet.addShort(drop->getPos().x);
	packet.addShort(drop->getPos().y);
	packet.addInt(0); //Time till
	packet.addShort(dropper.pos.x);
	packet.addShort(dropper.pos.y);
	packet.addShort(0);
	packet.addByte(0);
	if(!drop->getMesos()){
		packet.addBytes("8005BB46E6170200");
	}
	packet.sendTo(NULL, players, 1);
}

void DropsPacket::dropForPlayer(Player* player, Drop* drop, Dropped dropper){
	Packet packet = Packet();
	packet.addHeader(0xB9);
	packet.addByte(1);
	packet.addInt(drop->getObjID());
	packet.addByte(drop->getMesos());
	packet.addInt(drop->getID());
	packet.addInt(dropper.id);
	packet.addByte(0);
	packet.addShort(drop->getPos().x);
	packet.addShort(drop->getPos().y);
	packet.addInt(0); //Time till
	packet.addShort(dropper.pos.x);
	packet.addShort(dropper.pos.y);
	packet.addShort(0);
	packet.addByte(0);
	if(!drop->getMesos()){
		packet.addBytes("8005BB46E6170200");
	}
	packet.packetSend(player);
}

void DropsPacket::showDrop(Player* player, Drop* drop){
	Packet packet = Packet();
	packet.addHeader(0xBA);
	if(drop->getPlayer() == player->getPlayerid())
		packet.addByte(1);
	else
		packet.addByte(2);
	packet.addInt(drop->getObjID());
	packet.addByte(drop->getMesos());
	packet.addInt(drop->getID());
	packet.addInt(0); //TODO Dropper
	packet.addByte(0);
	packet.addShort(drop->getPos().x);
	packet.addShort(drop->getPos().y);
	packet.addInt(drop->getOwner());
	packet.addByte(0);
	if(!drop->getMesos()){
		packet.addBytes("8005BB46E6170200");
	}
	packet.packetSend(player);
}

void DropsPacket::takeNote(Player *player, int id, bool ismesos, short amount){
	Packet packet = Packet();
	packet.addHeader(0x32);
	packet.addByte(0);
	if(id==0)
		packet.addByte(-1);
	else{
		packet.addByte(ismesos);
		packet.addInt(id);
		if(ismesos){
			packet.addShort(0); // Internet Cafe Bonus
		}
		else if(id/1000000 != 1)
			packet.addShort(amount);
	}
	if(!ismesos){
		packet.addInt(0);
		packet.addInt(0);
	}
	packet.packetSend(player);
}

void DropsPacket::takeDrop(Player* player, vector <Player*> players, Drop* drop){
	Packet packet = Packet();
	packet.addHeader(0xBA);
	packet.addByte(2);
	packet.addInt(drop->getObjID());
	packet.addInt(player->getPlayerid());
	if(!drop->isQuest()){
		packet.sendTo(player, players, 1);
	}
	else{
		packet.packetSend(player);
	}
}

void DropsPacket::dontTake(Player* player){
	Packet packet = Packet();
	packet.addHeader(0x18);
	packet.addShort(1);
	packet.packetSend(player);
}

void DropsPacket::removeDrop(vector <Player*> players, Drop* drop){
	Packet packet = Packet();
	packet.addHeader(0xBA);
	packet.addByte(0);
	packet.addInt(drop->getObjID());
	packet.sendTo(NULL, players, 1);
}

void DropsPacket::explodeDrop(vector <Player*> players, Drop* drop){
	Packet packet = Packet();
	packet.addHeader(0xBA);
	packet.addByte(4);
	packet.addInt(drop->getObjID());
	packet.addShort(655);
	packet.sendTo(NULL, players, 1);
}