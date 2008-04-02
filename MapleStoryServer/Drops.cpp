#include "Drops.h"
#include "Mobs.h"
#include "Maps.h"
#include "DropsPacket.h"
#include "Inventory.h"
#include "Quests.h"

hash_map <int, MobDropsInfo> Drops::dropsinfo;
hash_map <int, Mesos> Drops::mesos;
hash_map <int, EquipInfo> Drops::equips;
hash_map <int, ItemInfo> Drops::items;
hash_map <int, FootholdsInfo> Drops::foots;
hash_map <int, int> Drops::objids;
hash_map <int, vector<Drop*>> Drops::drops;
hash_map <int, ConsumeInfo> Drops::consumes;

Drop* Drop::getDrop(int objid, int mapid){
	for(unsigned int i=0; i<Drops::drops[mapid].size(); i++){
		if(Drops::drops[mapid][i]->getObjID() == objid)
			return Drops::drops[mapid][i];
	}
	return NULL;
}

Pos Drops::findFloor(Pos pos, int map){
	short x = pos.x;
	short y = pos.y-100;
	bool first=1;
	short maxy;
	for(unsigned int i=0; i<Drops::foots[map].size(); i++){
		if((x>Drops::foots[map][i].x1 && x<Drops::foots[map][i].x2) || (x>Drops::foots[map][i].x2 && x<Drops::foots[map][i].x1)){
			if(first){
				maxy = (short) ( (float)( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) * x - Drops::foots[map][i].x1 * (float) ( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) + Drops::foots[map][i].y1 );
				if(maxy >= y)
					first=0;
			}
			else{
				short cmax = (short) ( (float)( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) * x - Drops::foots[map][i].x1 * (float) ( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) + Drops::foots[map][i].y1 );
				if(cmax < maxy && cmax >= y)
					maxy = cmax;
			}
		}
	}
	if(!first){
		Pos newpos;
		newpos.x = x;
		newpos.y = maxy;
		return newpos;
	}
	return pos;
}

void Drop::doDrop(Dropped dropped){
	setPos(Drops::findFloor(getPos(), getMap()));
	if(!isQuest())
		DropsPacket::drop(Maps::info[getMap()].Players, this, dropped);
	else{
		if(Players::players.find(playerid) == Players::players.end())
			return;
		if(Players::players[playerid]->getMap() == getMap())
			DropsPacket::dropForPlayer(Players::players[playerid], this, dropped);
	}
}

void Drop::takeDrop(Player* player){
	for(unsigned int i=0; i<Drops::drops[player->getMap()].size(); i++){
		if(Drops::drops[player->getMap()][i] == this){
			Drops::drops[player->getMap()].erase(Drops::drops[player->getMap()].begin()+i);
			break;
		}
	}
	DropsPacket::takeDrop(player, Maps::info[player->getMap()].Players, this);
}

void Drop::showDrop(Player* player){
	if(isQuest() && player->getPlayerid() != playerid)
		return;
	DropsPacket::showDrop(player, this);
}

void Drop::removeDrop(){

}

void Drops::dropMob(Player* player, Mob* mob){
	MobDropsInfo drop = dropsinfo[mob->getMobID()];
	int d=0;
	for(unsigned int k=0; k<drop.size(); k++){
		if(rand()%10000 < drop[k].chance){
			if(drop[k].quest>0){
				if(!player->quests->isQuestActive(drop[k].quest))
					continue;
				int request=0;
				for(unsigned int i=0; i<Quests::quests[drop[k].quest].rewards.size(); i++){
					if(Quests::quests[drop[k].quest].rewards[i].id == drop[k].id){
						request = Quests::quests[drop[k].quest].rewards[i].count;
					}
				}
				if(player->inv->getItemAmount(drop[k].id) > request)
					continue;
			}
			Drop* drp = new Drop(player->getMap());
			if(drop[k].quest>0){
				drp->setIsQuest(1);
				drp->setPlayer(player->getPlayerid());
				drp->setQuest(drop[k].quest);
			}
			drp->setID(drop[k].id);
			drp->setAmount(1);
			drp->setOwner(player->getPlayerid());
			if(drop[k].id/1000000 == 1){
				drp->setEquip(true);
				EquipInfo ei = equips[drp->getID()];
				DropInfo dp;
				dp.scrolls = 0;
				dp.type =  ei.type;
				dp.slots = ei.slots;
				dp.scrolls = 0;
				if(ei.istr > 0)
					dp.istr = ei.istr + rand()%3-1;
				else
					dp.istr = 0;
				if(ei.idex > 0)
					dp.idex = ei.idex + rand()%3-1;
				else
					dp.idex = 0;
				if(ei.iint > 0)
					dp.iint = ei.iint + rand()%3-1;
				else
					dp.iint = 0;
				if(ei.iluk > 0)
					dp.iluk = ei.iluk + rand()%3-1;
				else
					dp.iluk = 0;
				if(ei.ihp > 0)
					dp.ihp = ei.ihp + rand()%11-5;
				else
					dp.ihp = 0;
				if(ei.imp > 0)
					dp.imp = ei.imp + rand()%11-5;
				else
					dp.imp = 0;
				if(ei.iwatk > 0)
					dp.iwatk = ei.iwatk + rand()%11-5;
				else
					dp.iwatk = 0;
				if(ei.imatk > 0)
					dp.imatk = ei.imatk + rand()%11-5;
				else
					dp.imatk = 0;
				if(ei.iwdef > 0)
					dp.iwdef = ei.iwdef + rand()%11-5;
				else
					dp.iwdef = 0;
				if(ei.imdef > 0)
					dp.imdef = ei.imdef + rand()%11-5;
				else
					dp.imdef = 0;
				if(ei.iacc > 0)
					dp.iacc = ei.iacc + rand()%3-1;
				else
					dp.iacc = 0;
				if(ei.iavo > 0)
					dp.iavo = ei.iavo + rand()%3-1;
				else
					dp.iavo = 0;
				dp.ihand = ei.ihand;
				if(ei.ijump > 0)
					dp.ijump = ei.ijump + rand()%3-1;
				else
					dp.ijump = 0;
				if(ei.ispeed > 0)
					dp.ispeed = ei.ispeed + rand()%3-1;	
				else
					dp.ispeed = 0;
				drp->setDropInfo(dp);
			}
			drp->setTime(100);
			Dropped dropper;
			dropper.id = mob->getID();
			dropper.pos = mob->getPos();
			Pos pos;
			if(d%2){
				pos.x = mob->getPos().x+25*((d+1)/2);
				pos.y = mob->getPos().y;
			}
			else{
				pos.x = mob->getPos().x-25*(d/2);
				pos.y = mob->getPos().y;
			}
			drp->setPos(pos);
			drp->doDrop(dropper);
			d++;
		}
	}
	int nm = mesos[mob->getMobID()].min;
	int xm = mesos[mob->getMobID()].max;	
	if(xm > 0 && nm > 0){
		Drop* drp = new Drop(player->getMap());
		drp->setID(rand()%(xm-nm+1)+nm);
		drp->setMesos(true);
		drp->setOwner(player->getPlayerid());
		drp->setTime(100);
		Dropped dropper;
		dropper.id = mob->getID();
		dropper.pos = mob->getPos();
		Pos pos;
		if(d%2){
			pos.x = mob->getPos().x+25*((d+1)/2);
			pos.y = mob->getPos().y;
		}
		else{
			pos.x = mob->getPos().x-25*(d/2);
			pos.y = mob->getPos().y;
		}
		drp->setPos(pos);
		drp->doDrop(dropper);
	}
}

void Drops::showDrops(Player* player){
	for(unsigned int i=0; i<drops[player->getMap()].size(); i++){
		drops[player->getMap()][i]->showDrop(player);
	}
}

void Drops::lootItem(Player* player, unsigned char*packet){
	int itemid = getInt(packet+9);
	Drop* drop = Drop::getDrop(itemid, player->getMap());
	if(drop == NULL){
		DropsPacket::dontTake(player);
		return;
	}
	if(drop->isQuest()){
		int request=0;
		for(unsigned int i=0; i<Quests::quests[drop->getQuest()].rewards.size(); i++){
			if(Quests::quests[drop->getQuest()].rewards[i].id == drop->getID()){
				request = Quests::quests[drop->getQuest()].rewards[i].count;
			}
		}
		if(player->inv->getItemAmount(drop->getID()) > request || !player->quests->isQuestActive(drop->getQuest())){
			DropsPacket::takeNote(player, 0, 0, 0);
			DropsPacket::dontTake(player);
			return;
		}
	}
	if(drop->getMesos()){
		player->inv->setMesos(player->inv->getMesos() + drop->getID(), 1);
		DropsPacket::takeNote(player, drop->getID(), 1, 0);
	}
	else{
		if(drop->getEquip()){
			Equip* equip = new Equip;
			DropInfo ei = drop->getDropInfo();
			equip->id = drop->getID();
			equip->slots = ei.slots;
			equip->scrolls = ei.scrolls;
			equip->type = ei.type;
			equip->pos = Inventory::findSlot(player, drop->getID(), 1, 1);
			if(equip->pos == 0){
				DropsPacket::takeNote(player, 0, 0, 0);
				DropsPacket::dontTake(player);
				delete equip;
				return;
			}
			equip->istr = ei.istr;
			equip->idex = ei.idex;
			equip->iint = ei.iint;
			equip->iluk = ei.iluk;
			equip->ihp = ei.ihp;
			equip->imp = ei.imp;
			equip->iwatk = ei.iwatk;
			equip->imatk = ei.imatk;
			equip->iwdef = ei.iwdef;
			equip->imdef = ei.imdef;
			equip->iacc = ei.iacc;
			equip->iavo = ei.iavo;
			equip->ihand = ei.ihand;
			equip->ijump = ei.ijump;
			equip->ispeed = ei.ispeed;	
			Inventory::addEquip(player, equip, 1);
		}
		else{
			char type = Drops::items[drop->getID()].type;
			Item* newitem = new Item;
			newitem->amount = drop->getAmount();
			newitem->id = drop->getID();
			newitem->inv = type;
			newitem->pos = Inventory::findSlot(player, drop->getID() , type, drop->getAmount());
			if(newitem->pos == 0){
				DropsPacket::takeNote(player, 0, 0, 0);
				DropsPacket::dontTake(player);	
				delete newitem;
				return;
			}
			Inventory::addItem(player, newitem, 1);
		}
		DropsPacket::takeNote(player, drop->getID(), 0, drop->getAmount());
	}
	drop->takeDrop(player);
	delete drop;
}

void Drops::dropMesos(Player* player, unsigned char* packet){
	int amount = getInt(packet+4);
	player->inv->setMesos(player->inv->getMesos()-amount, 1);
	Drop* drop = new Drop(player->getMap());
	drop->setID(amount);
	drop->setMesos(true);
	drop->setOwner(player->getPlayerid());
	drop->setTime(0);
	Dropped dropper;
	dropper.id = player->getPlayerid();
	dropper.pos = player->getPos();
	drop->setPos(player->getPos());
	drop->doDrop(dropper);
}