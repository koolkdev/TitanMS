#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <stdio.h>
#include <string>

using namespace std;
#include "Tools.h"

class DropsData;
class EquipData;
class ItemData;
class MapData;
class MobData;
class QuestData;
class ShopData;
class ReactorData;
class SkillData;
class PetData;


class DataProvider {
private:
	static DataProvider* instance;
public:
	DataProvider(){
		instance = this;
	}
	static DataProvider* getInstance(){
		if(instance == NULL)
			new DataProvider();
		return instance;
	}
	DropsData* getMobDropData(int id);
	ReactorData* getReactorData(int id);
	EquipData* getEquipData(int id);
	ItemData* getItemData(int id);
	MapData* getMapData(int id);
	MobData* getMobData(int id);
	QuestData* getQuestData(int id);
	ShopData* getShopData(int id);
	SkillData* getSkillData(int id);
	PetData* getPetData(int id);
	int getItemMaxPerSlot(int id);
	int getItemPrice(int id);
	double getItemUnitPrice(int id);
	int getMobMaxMP(int id);
	int getMobMaxHP(int id);
	bool isMobBoss(int id);
	char getMobTagColor(int id);
	char getMobTagBgColor(int id);
	int getMobSummonDelay(int id);
	bool getMobFirstAttack(int id);
	int getReturnMap(int id);
	bool isItemBullet(int id);
	bool isItemCash(int id);
	int getMobExp(int id);
	int getItemMoveTo(int id);
	int getPetLife(int id);
	string getPetName(int id);
	bool getItemCash(int id);
	int getPetHungry(int id);
	int getReactorType(int id, int state);
	int getReactorNextState(int id, int state);
	string getReactorAction(int id);
	int getReactorDelay(int map, int id);
	Position getReactorLT(int id, int sid);
	Position getReactorRB(int id, int sid);
};


#endif