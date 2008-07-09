#include "PlayerInventories.h"
#include "Inventory.h"
#include "Item.h"
#include "Equip.h"
#include "ItemsData.h"
#include "Pet.h"
#include "EquipsData.h"
#include "PetsData.h"
#include "DataProvider.h"

PlayerInventories::PlayerInventories(Player* player){
	this->player = player;
	for(int i=0; i<6; i++){
		invs[i] = new Inventory(i, 100, player);
	}
	empty = new Inventory(-1, 0, player);
}
bool PlayerInventories::giveItem(int itemid, int amount, bool rands){
	if(!validID(itemid))
		return false;
	Item* item;
	if(IS_EQUIP(itemid))
		for(int i=0; i<amount; i++){
			item = new Equip(itemid, rands);
			bool check = getInventory(INVENTORY(itemid))-> addItem(item);
			if(!check)
				return false;
		}	
	else if(IS_PET(itemid))
		for(int i=0; i<amount; i++){
			item = new Pet(itemid);
			bool check = getInventory(INVENTORY(itemid))-> addItem(item);
			if(!check)
				return false;
		}	
	else{ 
		int max = DataProvider::getInstance()->getItemMaxPerSlot(itemid);
		while(amount > 0){
			if(amount%max == 0){
				item = new Item(itemid, max);
				amount -= max;
			}else{
				item = new Item(itemid, amount%max);
				amount -= amount%max;				
			}
			bool check = getInventory(INVENTORY(itemid))-> addItem(item);
			if(!check)
				return false;
		}
	}
	return true;
}
bool PlayerInventories::giveItem(Item* item, bool drop){
	return getInventory(INVENTORY(item->getID()))->addItem(item, true, drop);
}
Item* PlayerInventories::getItemBySlot(int inv, int slot){
	return getInventory(inv)->getItemBySlot(slot);
}
Item* PlayerInventories::getItemByID(int id){
	if(VALID_INVENTORY(INVENTORY(id)))
		return getInventory(INVENTORY(id))->getItemByID(id);
	return NULL;
}/*
void PlayerInventories::removeItem(int inv, int slot){
	getInventory(inv)->removeItem(slot);
	
}*/
void PlayerInventories::removeItem(int itemid){
	Item* item;
	while((item = getInventory(INVENTORY(itemid))->getItemByID(itemid)) != NULL)
		getInventory(INVENTORY(itemid))->removeItem(item, false, true);
	
}/*
void PlayerInventories::removeItem(Item* item){
	getInventory(INVENTORY(item->getID()))->removeItem(item);	
}*/

bool PlayerInventories::validID(int id){
	int inv = INVENTORY(id);
	if(IS_EQUIP(id))
		return (EquipsData::getInstance()->getDataByID(id) != NULL);
	else if(IS_PET(id))
		return (PetsData::getInstance()->getDataByID(id) != NULL);
	else
		return (ItemsData::getInstance()->getDataByID(id) != NULL);
	return false;
}

int PlayerInventories::getItemAmount(int id){
	return getInventory(INVENTORY(id))->getItemAmount(id);
}
void PlayerInventories::removeItemBySlot(int inv, int slot, int amount, bool send){
	getInventory(inv)->removeItemBySlot(slot, amount, send);
}
void PlayerInventories::deleteAll(){
	for(int i=0; i<6; i++){
		invs[i]->deleteAll();
	}
}