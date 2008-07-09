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



void PlayerHandler::usePetHandle(PacketReader* packet){
	packet->readInt();
	short slot = packet->readShort();
	Item* item = player->getInventories()->getItemBySlot(CASH, slot);
	if(item == NULL || MAJOR_TYPE(item->getID()) != 500){
		player->send(PacketCreator().enableAction());
		return;
	}
	Pet* pet = (Pet*)item;
	if(player->getPet() == pet){
		player->setPet(NULL);
	}
	else{
		pet->setStance(0);
		pet->setPosition(player->getPosition());
		player->setPet(pet);
	}
}
void PlayerHandler::movePetHandle(PacketReader* packet){
	int petid = packet->readInt();
	// TODO: multipets
	Pet* pet = player->getPet();
	if(pet == NULL || pet->getObjectID() != petid){
		return;
	}
	packet->readInt();
	Position start;
	start.x = packet->readShort();
	start.y = packet->readShort();
	ObjectMoving moving = ObjectMoving(packet);
	pet->setPosition(moving.getPosition());
	pet->setStance(moving.getStance());
	player->getMap()->send(PacketCreator().movePet(player->getID(), &moving, start), player);
}

void PlayerHandler::petCommandHandle(PacketReader* packet){
	int petid = packet->readInt();
	Pet* pet = player->getPet();
	if(pet == NULL || pet->getObjectID() != petid){
		return;
	}
	packet->read(5);
	int commandid = packet->read();
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
	player->send(PacketCreator().petCommandReplay(player->getID(), commandid, success));
}


void PlayerHandler::petCommandTextHandle(PacketReader* packet){
	int petid = packet->readInt();
	Pet* pet = player->getPet();
	if(pet == NULL || pet->getObjectID() != petid){
		return;
	}
	packet->read(5);
	char act = packet->read();
	string text = packet->readString();
	player->getMap()->send(PacketCreator().showPetText(player->getID(), text, 0, act), player);

}

