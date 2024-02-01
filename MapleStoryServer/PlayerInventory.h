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

#ifndef PINV_H
#define PINV_H

#include <vector>
using namespace std;

class Player;

struct Equip {
	int id;
	short pos;
	char slots;
	short scrolls;
	char type;
	short istr;
	short idex;
	short iint;
	short iluk;
	short ihp;
	short imp;
	short iwatk;
	short imatk;
	short iwdef;
	short imdef;
	short iacc;
	short iavo;
	short ihand;
	short ijump;
	short ispeed;
};

struct Item {
	short pos;
	char inv;
	int id;
	int amount;
};

class PlayerInventory {
public:
	PlayerInventory(){
		maxslots=100;
	}
	void setMesos(int mesos, bool is=0);
	void setMesosStart(int mesos){
		this->mesos=mesos;
	}
	int getMesos(){
		return this->mesos;
	}
	void addEquip(Equip* equip){
		equips.push_back(equip);
	}
	int getEquipNum(){
		return equips.size();
	}
	short getEquipPos(int equipid){
		return equips[equipid]->pos;
	}
	void setEquipPos(int equipid, short pos){
		equips[equipid]->pos = pos;
	}
	void deleteEquip(int equipid){
		delete equips[equipid];
		equips.erase(equips.begin()+equipid);
	}
	Equip* getEquip(int id){
		return equips[id];
	}
	void addItem(Item* item){
		items.push_back(item);
	}
	int getItemNum(){
		return items.size();
	}
	short getItemPos(int itemid){
		return items[itemid]->pos;
	}
	void setItemPos(int itemid, short pos){
		items[itemid]->pos = pos;
	}
	void deleteItem(int itemid){
		delete items[itemid];
		items.erase(items.begin()+itemid);
	}
	void setItem(Item* item, int itemid){
		items[itemid] = item;
	}
	Item* getItem(int id){
		return items[id];
	}
	void setPlayer(Player* player){
		this->player=player;
	}
	int getItemAmount(int itemid){
		int amount=0;
		for(int i=0; i<getItemNum(); i++)
			if(getItem(i)->id == itemid)
				amount+=getItem(i)->amount;
		return amount;
	}
	int getItemBySlot(int itemid, int slot, char inv){
		int amount=0;
		for(int i=0; i<getItemNum(); i++)
			if(getItem(i)->id == itemid && getItem(i)->pos == slot && getItem(i)->inv == inv){
				amount=getItem(i)->amount;
				break;
			}
		return amount;
	}
private:
	int maxslots;
	int mesos;
	Player* player;
	vector <Equip*> equips;
	vector <Item*> items;
};

#endif