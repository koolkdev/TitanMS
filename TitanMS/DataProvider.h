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
	int getMobLevel(int id);
	int getMobWAtk(int id);
	bool isMobBoss(int id);
	char getMobTagColor(int id);
	char getMobTagBgColor(int id);
	bool getMobFirstAttack(int id);
	bool getMobDamagedByMob(int id);
	int getReturnMap(int id);
	bool isItemBullet(int id);
	bool isItemCash(int id);
	int getMobExp(int id);
	int getItemMoveTo(int id);
	int getPetLife(int id);
	string getItemName(int id);
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