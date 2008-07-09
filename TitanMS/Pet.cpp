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
			*pet->getTimer() = -1;
			player->setPet(NULL);
			pet->addCloseness(-20);
			pet->setFullness(5);
			player->send(PacketCreator().showMassage("The pet was hungry so it went back to its home.",5));
		}
		else{
			*pet->getTimer() = Timer::getInstance()->setTimer(delay, new PetFullnessTimer(player, pet, delay));
		}
		player->send(PacketCreator().updatePet(pet));
	}
};

int Pet::ids=0x100;

int Pet::levels [30] = {1, 3, 6, 14, 31, 60, 108, 181, 287, 434, 632, 891, 1224, 1642, 2161, 2793, 3557, 4467, 5542, 6801, 8263, 9950, 11882, 14084, 16578, 19391, 22548, 26074, 30000, 0};

Pet::Pet(int id){
	timer = -1;
	lastTime = 0;
	type = 5;
	((Item*)this)->setID(id);
	amount = 1;
	slot=0;
	lock = false;
	time = getTimeByDays(DataProvider::getInstance()->getPetLife(id));
	((MapObject*)this)->setID(ids++);
	name = DataProvider::getInstance()->getPetName(id);
	level = 1;
	closeness = 0;
	fullness = 100;
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
			player->send(PacketCreator().showPlayerEffect(Player::Effects::PET_LEVEL_UP));
			player->getMap()->send(PacketCreator().showEffect(player->getID(), Player::Effects::PET_LEVEL_UP), player);
		}
	}
	if(player != NULL)
		player->send(PacketCreator().updatePet(this, false));
}
void Pet::startTimer(Player* player){
	timer = Timer::getInstance()->setTimer(5*60*1000/DataProvider::getInstance()->getPetHungry(getItemID()), new PetFullnessTimer(player, this, 5*60*1000/DataProvider::getInstance()->getPetHungry(getItemID())));
}
void Pet::stopTimer(){
	if(timer != -1)
		Timer::getInstance()->cancelTimer(timer);

}