#include "DataProvider.h"
#include "DropsData.h"
#include "EquipsData.h"
#include "ItemsData.h"
#include "MapsData.h"
#include "MapMobData.h"
#include "MapMobsData.h"
#include "MapReactorData.h"
#include "MapReactorsData.h"
#include "MobsDropData.h"
#include "MobsData.h"
#include "QuestsData.h"
#include "ShopsData.h"
#include "SkillsData.h"
#include "PetsData.h"
#include "PetData.h"
#include "DropData.h"
#include "EquipData.h"
#include "PetData.h"
#include "ItemData.h"
#include "MapData.h"
#include "MobData.h"
#include "QuestData.h"
#include "ShopData.h"
#include "ReactorData.h"
#include "ReactorsData.h"
#include "ReactorStateData.h"
#include "SkillData.h"

DataProvider* DataProvider::instance = NULL;

DropsData* DataProvider::getMobDropData(int id){
	return MobsDropData::getInstance()->getDataByID(id);
}
EquipData* DataProvider::getEquipData(int id){
	return EquipsData::getInstance()->getDataByID(id);
}
ItemData* DataProvider::getItemData(int id){
	return ItemsData::getInstance()->getDataByID(id);
}
ReactorData* DataProvider::getReactorData(int id){
	return ReactorsData::getInstance()->getDataByID(id);
}
MapData* DataProvider::getMapData(int id){
	return MapsData::getInstance()->getDataByID(id);
}
MobData* DataProvider::getMobData(int id){
	return MobsData::getInstance()->getDataByID(id);
}
QuestData* DataProvider::getQuestData(int id){
	return QuestsData::getInstance()->getDataByID(id);
}
ShopData* DataProvider::getShopData(int id){
	return ShopsData::getInstance()->getDataByID(id);
}
SkillData* DataProvider::getSkillData(int id){
	return SkillsData::getInstance()->getDataByID(id);
}
PetData* DataProvider::getPetData(int id){
	return PetsData::getInstance()->getDataByID(id);
}
int DataProvider::getItemMaxPerSlot(int id){
	ItemData* item = getItemData(id);
	if(item == NULL)
		return 1;
	return item->getMaxPerSlot();
}
int DataProvider::getItemPrice(int id){
	ItemData* item = getItemData(id);
	if(item == NULL){
		EquipData* equip = getEquipData(id);
		if(equip == NULL)
			return 0;
		return equip->getPrice();
	}
	return item->getPrice();
}
bool DataProvider::isItemCash(int id){
	EquipData* equip = getEquipData(id);
	if(equip == NULL)
		return 0;
	return equip->getCash();
}
double DataProvider::getItemUnitPrice(int id){
	ItemData* item = getItemData(id);
	if(item == NULL)
		return 0;
	return item->getUnitPrice();
}
int DataProvider::getMobMaxMP(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getMaxMP();
}
int DataProvider::getMobMaxHP(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getMaxHP();
}
bool DataProvider::isMobBoss(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getIsBoss();
}
char DataProvider::getMobTagColor(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getColor();
}
char DataProvider::getMobTagBgColor(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getBgColor();
}
int DataProvider::getMobSummonDelay(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getDieDelay();
}
bool DataProvider::getMobFirstAttack(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getFirstAttack();
}

int DataProvider::getReturnMap(int id){
	MapData* map = getMapData(id);
	if(map == NULL)
		return 0;
	return map->getReturnMap();

}
bool DataProvider::isItemBullet(int id){
	ItemData* item = getItemData(id);
	if(item == NULL)
		return 0;
	return item->getBullet();
}
int DataProvider::getMobExp(int id){
	MobData* mob = getMobData(id);
	if(mob == NULL)
		return 0;
	return mob->getExp();
}
int DataProvider::getItemMoveTo(int id){
	ItemData* item = getItemData(id);
	if(item == NULL)
		return 0;
	return item->getEffectData()->getMoveTo();
}
int DataProvider::getPetLife(int id){
	PetData* pet = getPetData(id);
	if(pet == NULL)
		return 0;
	return pet->getLife();
}
int DataProvider::getPetHungry(int id){
	PetData* pet = getPetData(id);
	if(pet == NULL)
		return 0;
	return pet->getHungry();
}
string DataProvider::getPetName(int id){
	PetData* pet = getPetData(id);
	if(pet == NULL)
		return "";
	return pet->getName();
}
bool DataProvider::getItemCash(int id){
	ItemData* item = getItemData(id);
	if(item == NULL){
		EquipData* equip = getEquipData(id);
		if(equip == NULL){
			PetData* pet = getPetData(id);
			return pet != NULL;
		}
		return equip->getCash();
	}
	//TODO: is item cash
	return false;
}

int DataProvider::getReactorType(int id, int state){
	ReactorData* reactor = getReactorData(id);
	if(reactor == NULL)
		return -1;
	ReactorStateData* s = reactor->getDataByID(state);
	if(s == NULL)
		return -1;
	return s->getType();
}
int DataProvider::getReactorNextState(int id, int state){
	ReactorData* reactor = getReactorData(id);
	if(reactor == NULL)
		return -1;
	ReactorStateData* s = reactor->getDataByID(state);
	if(s == NULL)
		return -1;
	return s->getNextState();
}
string DataProvider::getReactorAction(int id){
	ReactorData* reactor = getReactorData(id);
	if(reactor == NULL)
		return "";
	return reactor->getAction();
}
int DataProvider::getReactorDelay(int map, int id){
	MapData* m = getMapData(map);
	if(m == NULL)
		return 0;
	vector <MapReactorData*>* r= m->getReactorsData()->getData();
	for(int i=0; i<(int)r->size(); i++){
		if((*r)[i]->getInMapID() == id){
			return (*r)[i]->getDelay();
		}
	}
	return 0;
}
Position DataProvider::getReactorLT(int id, int sid){
	Position empty;
	empty.x=0;
	empty.y=0;
	ReactorData* reactor = getReactorData(id);
	if(reactor == NULL)
		return empty;
	ReactorStateData* s = reactor->getDataByID(sid);
	if(s == NULL)
		return empty;
	return s->getLT();
}
Position DataProvider::getReactorRB(int id, int sid){
	Position empty;
	empty.x=0;
	empty.y=0;
	ReactorData* reactor = getReactorData(id);
	if(reactor == NULL)
		return empty;
	ReactorStateData* s = reactor->getDataByID(sid);
	if(s == NULL)
		return empty;
	return s->getRB();
}