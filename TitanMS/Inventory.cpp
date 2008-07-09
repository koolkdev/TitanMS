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

#include "Inventory.h"
#include "PlayerInventories.h"
#include "DataProvider.h"
#include "Item.h"
#include "PacketCreator.h"
#include "Player.h"

bool Inventory::addItem(Item* item, bool findslot, bool drop){
	if(findslot){
		int pos = 0;
		for(bool j=false, first = true; j || first; j=!j, first = false){
			for(int i=1; i<=maxslots; i++){
				if(items.find(i) == items.end() || items[i] == NULL){
					if(j){
						item->setSlot(i);
						items[i] = item;
						player->send(PacketCreator().newItem(item, drop));
						return true;
					}
				}
				else {
					if(items[i]->getID() == item->getID() && items[i]->getAmount() < DataProvider::getInstance()->getItemMaxPerSlot(item->getID())){
						if(item->getAmount() + items[i]->getAmount() <= DataProvider::getInstance()->getItemMaxPerSlot(item->getID())){
							items[i]->setAmount(items[i]->getAmount()+item->getAmount());
							player->send(PacketCreator().updateSlot(items[i], drop));
							delete item;
							return true;
						}
						else{
							int change = DataProvider::getInstance()->getItemMaxPerSlot(item->getID()) - items[i]->getAmount();
							item->setAmount(item->getAmount()-change);
							items[i]->setAmount(DataProvider::getInstance()->getItemMaxPerSlot(item->getID()));
							player->send(PacketCreator().updateSlot(items[i], drop));										
						}
					}
				}
			}		
		}
	}
	else{
		items[item->getSlot()] = item;
		return true;
	}
	return false;
}

Item* Inventory::getItemBySlot(int slot){
	if(items.find(slot) == items.end())
		return NULL;
	return items[slot];
}
Item* Inventory::getItemByID(int id){
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if(iter->second != NULL && iter->second->getID() == id){
			return iter->second;
		}
	}
	return NULL;
}
void Inventory::removeItem(int slot, bool drop, bool del, bool send){
	if(items.find(slot) == items.end())
		return;
	if(send){
		if(drop) player->send(PacketCreator().moveItem(inv, slot, 0));
		else player->send(PacketCreator().removeItem(items[slot]->getType(), slot, drop));
	}
	if(del) delete items[slot];
	items.erase(slot);
}
void Inventory::removeItem(Item* item, bool drop, bool del, bool send){
	if(item != NULL)
		removeItem(item->getSlot(), drop, del, send);
}
int Inventory::getItemAmount(int itemid){
	int amount = 0;
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if(iter->second != NULL && iter->second->getID() == itemid){
			amount += iter->second->getAmount();
		}
	}
	return amount;
}
Item* Inventory::getItemByType(int type){
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if(iter->second != NULL && iter->second->getID()%10000 == type){
			return iter->second;
		}
	}
	return NULL;
}

void Inventory::removeItem(int itemid, int amount){
	vector <Item*> todel;
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if(iter->second != NULL && iter->second->getID() == itemid){
			if(iter->second->getAmount() < amount){
				amount -= iter->second->getAmount();
				todel.push_back(iter->second);
			}
			else{
				iter->second->setAmount(iter->second->getAmount()-amount);
				player->send(PacketCreator().updateSlot(iter->second, true));
			}
		}
	}
	for(int i=0; i<(int)todel.size(); i++){
		removeItem(todel[i], true, true);
	}
}
void Inventory::removeItemBySlot(int slot, int amount, bool send){
	if(items.find(slot) == items.end())
		return;
	items[slot]->setAmount(items[slot]->getAmount()-amount);
	if(items[slot]->getAmount() <= 0){
		removeItem(slot, true, true, send);
	}
	else{
		player->send(PacketCreator().updateSlot(items[slot], true));
	}
	
}

void Inventory::deleteAll(){
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++)
		delete iter->second;
}