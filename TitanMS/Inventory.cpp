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
#include "Pet.h"
#include "Equip.h"
#include "PacketCreator.h"
#include "Player.h"

bool Inventory::addItem(Item* item, bool findslot, bool drop, bool send){
	short oAmount = item->getAmount();
	if(findslot){
		int pos = 0;
		for(bool j=false, first = true; j || first; j=!j, first = false){
			for(int i=1; i<=maxslots; i++){
				if(items.find(i) == items.end() || items[i] == NULL){
					if(j){
						item->setSlot(i);
						if(send){
							items[i] = item;
							player->send(PacketCreator().newItem(item, drop));
						}
						return true;
					}
				}
				else {
					if(items[i]->getID() == item->getID() && !IS_STAR(item->getID()) && items[i]->getAmount() < DataProvider::getInstance()->getItemMaxPerSlot(item->getID())){
						if(item->getAmount() + items[i]->getAmount() <= DataProvider::getInstance()->getItemMaxPerSlot(item->getID())){
							if(send){
								items[i]->setAmount(items[i]->getAmount()+item->getAmount());
								player->send(PacketCreator().updateSlot(items[i], drop));
								delete item;
							}
							else
								item->setAmount(oAmount);
							return true;
						}
					  	else{
							int change = DataProvider::getInstance()->getItemMaxPerSlot(item->getID()) - items[i]->getAmount();
							item->setAmount(item->getAmount()-change);
							if(send){
								items[i]->setAmount(DataProvider::getInstance()->getItemMaxPerSlot(item->getID()));
								player->send(PacketCreator().updateSlot(items[i], drop));										
							}
						}
					}
				}
			}		
		}
	}
	else{
		if(items.find(item->getSlot()) != items.end()){
			if(items[item->getSlot()]->getID() == item->getID()){
				item->setAmount(items[item->getSlot()]->getAmount() + item->getAmount());
				if(item->getAmount() > DataProvider::getInstance()->getItemMaxPerSlot(item->getID()))
					item->setAmount(DataProvider::getInstance()->getItemMaxPerSlot(item->getID()));
				if(send)
					player->send(PacketCreator().updateSlot(item, drop));
				delete items[item->getSlot()];
			}
			else{
				delete items[item->getSlot()];
				if(send)
					player->send(PacketCreator().newItem(item, drop));
			}
		}
		else{
			if(send)
				player->send(PacketCreator().newItem(item, drop));
		}
		items[item->getSlot()] = item;
		return true;
	}
	if(!send)
		item->setAmount(oAmount);
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
	Item* min = NULL;
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if((min == NULL || (min != NULL && iter->first < min->getSlot())) && iter->second->getID()/10000 == type){
			min = iter->second;
		}
	}
	return min;
}

void Inventory::removeItem(int itemid, int amount){
	vector <Item*> todel;
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end() && amount; iter++){
		if(iter->second != NULL && iter->second->getID() == itemid){
			if(iter->second->getAmount() <= amount){
				amount -= iter->second->getAmount();
				todel.push_back(iter->second);
				if(amount == 0) break;
			}
			else{
				iter->second->setAmount(iter->second->getAmount()-amount);
				player->send(PacketCreator().updateSlot(iter->second, true));
				break;
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
	if(items[slot]->getAmount() <= 0 && !IS_STAR(items[slot]->getID())){
		removeItem(slot, true, true, send);
	}
	else{
		if(items[slot]->getAmount() < 0){
			items[slot]->setAmount(0);
		}
		player->send(PacketCreator().updateSlot(items[slot], true));
	}
	
}

void Inventory::deleteAll(){
	for (hash_map<int,Item*>::iterator iter = items.begin(); iter != items.end(); iter++){
		if(IS_EQUIP(iter->second->getID()))
			delete (Equip*)iter->second;
		else if(IS_PET(iter->second->getID()))
			delete (Pet*)iter->second;
		else
			delete iter->second;

	}
}