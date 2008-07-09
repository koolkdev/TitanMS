#include "Drop.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "PacketCreator.h"
#include "MapDrops.h"
#include "PlayerInventories.h"

Drop::Drop(Map* map, Item* item){
	this->map = map;
	this->item = item;
	owner = 0;
	mesos = false;
	quest = false;
	questid = 0;
	exptime = 0;
}
Drop::Drop(Map* map, Item* item, Position pos){
	this->pos = pos;
	this->map = map;
	this->item = item;
	owner = 0;
	mesos = false;
	quest = false;
	questid = 0;
	exptime = 0;
}
Drop::Drop(Map* map){
	this->map = map;
	item = NULL;
	owner = 0;
	mesos = false;
	quest = false;
	questid = 0;
	exptime = 0;
}
void Drop::drop(MapObject* dropper){
	map->getDrops()->addDrop(this);
	map->send(PacketCreator().dropObject(this, 1, dropper));
}
void Drop::showDrop(Player* player){
	player->send(PacketCreator().showDrop(this));
}
void Drop::takeDrop(Player* player){
	// TODO: delete drops
	if(!mesos){
		int amount = item->getAmount();
		int itemid = item->getID();
		if(player->getInventories()->giveItem(item, true)){
			map->send(PacketCreator().removeDrop(id, 2, player->getID()));
			player->send(PacketCreator().gainDrop(itemid, amount));
		}
		else {
			if(amount != item->getAmount()){
				// TODO: add to mapdrops and fix
				Drop drop = Drop(map, &Item(item->getID()));
				drop.setID(MapDrops::ids++);
				map->send(PacketCreator().showDrop(&drop));
				map->send(PacketCreator().lootDrop(getID(), player->getID()));
				player->send(PacketCreator().gainDrop(itemid, amount-item->getAmount()));
			}
			else{
				player->send(PacketCreator().emptyInventoryAction());
				player->send(PacketCreator().lootError());
			}
		}
	}
	else{
		player->addMesos(amount, true, true);
		map->send(PacketCreator().removeDrop(id, 2, player->getID()));
		player->send(PacketCreator().gainDrop(amount, amount, true));
	}
}
void Drop::removeDrop(bool explode){
	map->send(PacketCreator().removeDrop(id, (explode) ? 4 : 0));
}