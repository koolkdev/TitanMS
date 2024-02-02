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

#include "DataProvider.h"
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
	dropTime = 0;
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
	dropTime = 0;
}
Drop::Drop(Map* map){
	this->map = map;
	item = NULL;
	owner = 0;
	mesos = false;
	quest = false;
	questid = 0;
	exptime = 0;
	dropTime = 0;
}
void Drop::drop(MapObject* dropper, int delay){
	map->getDrops()->addDrop(this);
	map->send(PacketCreator().dropObject(this, 1, dropper, delay));
}
void Drop::showDrop(Player* player){
	player->send(PacketCreator().showDrop(this));
}
bool Drop::takeDrop(Player* player){
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
				// TODO: add to MapDrops and fix
				Drop drop = Drop(map, &Item(item->getID()));
				drop.setID(MapDrops::ids++);
				map->send(PacketCreator().showDrop(&drop));
				map->send(PacketCreator().lootDrop(drop.getID(), player->getID()));
				player->send(PacketCreator().gainDrop(itemid, amount-item->getAmount()));
			}
			else{
				player->send(PacketCreator().emptyInventoryAction());
				player->send(PacketCreator().lootError());
			}
			return false;
		}
	}
	else{
		player->addMesos(amount, true, true);
		map->send(PacketCreator().removeDrop(id, 2, player->getID()));
		player->send(PacketCreator().gainDrop(amount, amount, true));
	}
	return true;
}
void Drop::removeDrop(bool explode){
	map->send(PacketCreator().removeDrop(id, (explode) ? 4 : 0));
}