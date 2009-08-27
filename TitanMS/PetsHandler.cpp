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

#include <windows.h>
#include "DataProvider.h"
#include "PlayerHandler.h"
#include "PacketReader.h" 
#include "PacketCreator.h" 
#include "Pet.h"
#include "Inventory.h"
#include "PlayerInventories.h"
#include "Player.h"
#include "ObjectMoving.h"
#include "PetData.h"
#include "PetsData.h"
#include "Map.h"
#include "PetCommandData.h"
#include "Tools.h"
using namespace Tools;



void PlayerHandler::usePetHandle(PacketReader& packet){
	// to fix multipets, check if player got the skill 8
	packet.readInt();
	short slot = packet.readShort();
	Item* item = player->getInventories()->getItemBySlot(CASH, slot);
	if(item == NULL || MAJOR_TYPE(item->getID()) != 500){
		player->send(PacketCreator().enableAction());
		return;
	}
	Pet* pet = (Pet*)item;
	Pet* ppet = player->getPet(pet->getObjectID());
	if(ppet != NULL){
		player->removePet(pet);
	}
	else{
		player->addPet(pet);
	}
}
void PlayerHandler::movePetHandle(PacketReader& packet){
	int petid = packet.readInt();
	// TODO: multipets
	Pet* pet = player->getPet(petid);
	if(pet == NULL){
		return;
	}
	packet.readInt();
	Position start;
	start.x = packet.readShort();
	start.y = packet.readShort();
	ObjectMoving moving = ObjectMoving(packet, pet);
	player->getMap()->send(PacketCreator().movePet(player->getID(), pet->getPetSlot(), moving, start), player);
}

void PlayerHandler::petCommandHandle(PacketReader& packet){
	int petid = packet.readInt();
	Pet* pet = player->getPet(petid);
	if(pet == NULL){
		return;
	}
	packet.read(5);
	int commandid = packet.read();
	PetData* data = PetsData::getInstance()->getDataByID(pet->getItemID());
	if(data == NULL) return;
	PetCommandData* command = data->getDataByID(commandid);
	if(command == NULL) return;
	if(pet->getLevel() < command->getMinLevel() || pet->getLevel() > command->getMaxLevel()) return;
	bool success = pet->getLastTime() + 45*1000/(pet->getLevel()/10+1) < (int)GetTickCount() && random(100) < command->getProb();
	if(success){ 
		pet->addCloseness(command->getCloseness(), player); 
		pet->setLastTime(GetTickCount());
	}
	player->send(PacketCreator().petCommandReplay(player->getID(), pet->getPetSlot(), commandid, success));
}


void PlayerHandler::petCommandTextHandle(PacketReader& packet){
	int petid = packet.readInt();
	Pet* pet = player->getPet(petid);
	if(pet == NULL){
		return;
	}
	packet.read(5);
	char act = packet.read();
	string text = packet.readString();
	player->getMap()->send(PacketCreator().showPetText(player->getID(), pet->getPetSlot(), text, act), player);

}

