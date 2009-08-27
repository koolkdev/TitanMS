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

#ifndef PLAYERINVENOTRYs_H
#define PLAYERINVENOTRYs_H

#include <hash_map>
using namespace std;
using namespace stdext;

#define INVENTORY(x) (x/1000000)
#define IS_EQUIP(x) (INVENTORY(x) == 1)
#define VALID_INVENTORY(x) (x>=1 && x<=5)
#define TYPE(x) (x/1000000)
#define MAJOR_TYPE(x) (x/10000)
#define MINOR_TYPE(x) ((x/1000)%10)
#define IS_STAR(x) (MAJOR_TYPE(x) == 207 || MAJOR_TYPE(x) == 233) // TODO: or IS_THROWABLE?
#define IS_PET(x) (MAJOR_TYPE(x) == 500)

#define EQUIPPED 0
#define EQUIP 1
#define USE 2
#define SETUP 3
#define ETC 4
#define CASH 5

class Inventory;
class Item;
class Player;

struct CompareItems : public std::binary_function<Item*, Item*, bool>
{
bool operator()(Item* x, Item* y) const;
};
class PlayerInventories {
private:
	hash_map <int, Inventory*> invs;
	Player* player;
	Inventory* empty;
public:
	PlayerInventories(Player* player);
	Inventory* getInventory(int n){
		if(invs.find(n) != invs.end())
			return invs[n];
		return empty;
	}
	bool giveItem(int itemid, int amount=1, bool rands = false);
	bool giveItem(Item* item, bool drop = false);
	Item* getItemBySlot(int inv, int slot);
	Item* getItemByID(int id);
	//void removeItem(int inv, int slot);
	void removeItem(int itemid);
	//void removeItem(Item* item);
	static bool validID(int id);
	int getItemAmount(int id);
	void removeItemBySlot(int inv, int slot, int amount, bool send = true);
	void deleteAll();
};

#endif