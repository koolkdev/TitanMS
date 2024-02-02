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

#include "DataProvider.h"
#include "Pet.h"
#include "PetsData.h"
#include "PetData.h"
#include "DataProvider.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Map.h"
#include "Run.h"
#include "Timer.h"
#include "Tools.h"
using namespace Tools;

class PetFullnessTimer : public Run{
private:
	Pet* pet;
	Player* player;
	int delay;
public:
	PetFullnessTimer(Player* player, Pet* pet, int delay):pet(pet), player(player), delay(delay){}
	void run(){
		pet->setFullness(pet->getFullness()-1);
		if(pet->getFullness() == 0){
			*pet->getTimer() = NULL;
			player->removePet(pet, true);
			pet->addCloseness(-20);
			pet->setFullness(5);
		}
		else{
			*pet->getTimer() = Timers::getInstance()->startTimer(delay, new PetFullnessTimer(player, pet, delay));
		}
		player->send(PacketCreator().updatePet(pet));
	}
};

int Pet::ids=0x100;

int Pet::levels [30] = {1, 3, 6, 14, 31, 60, 108, 181, 287, 434, 632, 891, 1224, 1642, 2161, 2793, 3557, 4467, 5542, 6801, 8263, 9950, 11882, 14084, 16578, 19391, 22548, 26074, 30000, 0};

Pet::Pet(int id){
	timer = NULL;
	lastTime = 0;
	type = 5;
	((Item*)this)->setID(id);
	amount = 1;
	slot=0;
	petSlot = -1;
	lock = false;
	time = getTimeByDays(DataProvider::getInstance()->getPetLife(id));
	((MapObject*)this)->setID(ids++);
	name = DataProvider::getInstance()->getItemName(id);
	level = 1;
	closeness = 0;
	fullness = 100;
}
Pet::~Pet(){
	stopTimer();
}
void Pet::setCloseness(short closeness, Player* player){
	if(closeness > 30000)
		closeness = 30000;
	if(closeness < 0)
		closeness = 0;
	this->closeness = closeness;
	if(closeness >= levels[level-1] && level < 30){
		while(closeness >= levels[level-1]){
			level++;
			if(level == 30)
				break;
		}
		if(player != NULL){
			player->send(PacketCreator().showPlayerEffect(Player::Effects::PET_LEVEL_UP, petSlot));
			player->getMap()->send(PacketCreator().showEffect(player->getID(), Player::Effects::PET_LEVEL_UP, petSlot), player);
		}
	}
	if(player != NULL)
		player->send(PacketCreator().updatePet(this));
}
void Pet::startTimer(Player* player){
	int petHunger = DataProvider::getInstance()->getPetHungry(getItemID());
	if( petHunger == 0 ) petHunger = 1;
	timer = Timers::getInstance()->startTimer(5*60*1000/petHunger, new PetFullnessTimer(player, this, 5*60*1000/petHunger));
}

void Pet::stopTimer(){
	if(timer != NULL)
		Timers::getInstance()->cancelTimer(timer);

}