#include "Item.h"
#include "PlayerInventories.h"
#include "DataProvider.h"
#include "ItemData.h"

Item::Item(int itemid, int amount){
	type = INVENTORY(itemid);
	id = itemid;
	this->amount = amount;
	slot=0;
	lock = false;
} 
Item::Item(){
	lock = false;
	type=-1;
	slot=0;
}
void Item::setID(int id){
	this->id = id;
	type = INVENTORY(id);
}
bool Item::isBullet(){
	return DataProvider::getInstance()->isItemBullet(id);
}