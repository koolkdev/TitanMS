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

#ifndef PINV_H
#define PINV_H

#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

class Item;
class Player;

class Inventory {
public:
	Inventory(char inv, int slots, Player* player){
		this->inv = inv;
		maxslots=slots;
		this->player = player;
	}
	Item* getItemBySlot(int slot);
	Item* getItemByID(int id);
	void removeItem(int slot, bool drop = false, bool del = false, bool send = true);
	void removeItem(Item* item, bool drop = false, bool del = false, bool send = true);
	bool addItem(Item* item, bool findslot = true, bool drop = false, bool send = true);
	hash_map <int, Item*>* getItems(){
		return &items;
	}
	int getSlots(){
		return maxslots;
	}
	int getItemAmount(int itemid);
	Item* getItemByType(int type);
	void removeItem(int itemid, int amount);
	void removeItemBySlot(int slot, int amount = 1, bool send = true);
	void deleteAll();
private:
	char inv;
	Player* player;
	int maxslots;
	hash_map <int, Item*> items;
};

#endif