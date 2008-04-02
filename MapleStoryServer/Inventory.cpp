#include "Inventory.h"
#include "Player.h"
#include "InventoryPacket.h"
#include "Drops.h"
#include "Shops.h"
#include "Quests.h"
#include "Maps.h"

void Inventory::itemMove(Player* player, unsigned char* packet){
	char inv = packet[4];
	short slot1 = getShort(packet+5);
	short slot2 = getShort(packet+7);
	if(inv == 1){
		if(slot2 == 0){
			InventoryPacket::moveItem(player, inv, slot1, slot2);
			Equip* equip;
			int num;
			for(int i=0; i<player->inv->getEquipNum(); i++){
				if(player->inv->getEquipPos(i) == slot1){
					equip = player->inv->getEquip(i);	
					num = i;
					break;
				}
			}
			if(equip==NULL)
				return;
			Drop* drop = new Drop(player->getMap());
			drop->setID(equip->id);
			drop->setOwner(player->getPlayerid());
			drop->setTime(0);
			drop->setEquip(true);
			Dropped dropper;
			dropper.id = player->getPlayerid();
			dropper.pos = player->getPos();
			drop->setPos(player->getPos());
			DropInfo dropi;
			dropi.type = equip->type;
			dropi.scrolls = equip->scrolls;
			dropi.slots = equip->slots;
			dropi.istr = equip->istr;
			dropi.idex = equip->idex;
			dropi.iint = equip->iint;
			dropi.iluk = equip->iluk;
			dropi.ihp = equip->ihp;
			dropi.imp = equip->imp;
			dropi.iwatk = equip->iwatk;
			dropi.imatk = equip->imatk;
			dropi.iwdef = equip->iwdef;
			dropi.imdef = equip->imdef;
			dropi.iacc = equip->iacc;
			dropi.iavo = equip->iavo;
			dropi.ihand = equip->ihand;
			dropi.ijump = equip->ijump;
			dropi.ispeed = equip->ispeed;
			drop->setDropInfo(dropi);
			drop->doDrop(dropper);
			player->inv->deleteEquip(num);
			player->quests->checkQuests();
			return;
		}
		for(int i=0; i<player->inv->getEquipNum(); i++){

			if(player->inv->getEquipPos(i) == slot1){
				for(int j=0; j<player->inv->getEquipNum(); j++){
					if(player->inv->getEquipPos(j) == slot2){
						player->inv->setEquipPos(j, slot1);
						break;
					}
				}		
				player->inv->setEquipPos(i, slot2);	
				break;
			}
		}	
		InventoryPacket::moveItem(player, inv, slot1, slot2);
		InventoryPacket::updatePlayer(player);
	}
	else {
		if(slot2 == 0){
			short amount = getShort(packet+9);
			Item* item;
			int num;
			for(int i=0; i<player->inv->getItemNum(); i++){
				if(player->inv->getItemPos(i) == slot1 && player->inv->getItem(i)->inv == inv){
					item = player->inv->getItem(i);	
					num = i;
					break;
				}
			}
			if(item==NULL)
				return;
			if(item->amount - amount == 0){
				item->amount = 0;
				InventoryPacket::moveItem(player, inv, slot1, slot2);
			}
			else{
				item->amount -= amount;
				InventoryPacket::moveItemS(player, inv, slot1, item->amount);
			}
			Drop* drop = new Drop(player->getMap());
			drop->setID(item->id);
			drop->setOwner(player->getPlayerid());
			drop->setTime(0);
			drop->setAmount(amount);
			Dropped dropper;
			dropper.id = player->getPlayerid();
			dropper.pos = player->getPos();
			drop->setPos(player->getPos());
			drop->doDrop(dropper);
			if(item->amount == 0)
				player->inv->deleteItem(num);
			player->quests->checkQuests();
			return;
		}
		int stack=0;
		int amount1=0;
		int amount2=0;
		for(int i=0; i<player->inv->getItemNum(); i++){
			if(player->inv->getItemPos(i) == slot1 && player->inv->getItem(i)->inv == inv){
				for(int j=0; j<player->inv->getItemNum(); j++){
					if(player->inv->getItemPos(j) == slot2 && player->inv->getItem(j)->inv == inv){
						if(player->inv->getItem(i)->id == player->inv->getItem(j)->id){
							if(player->inv->getItem(j)->amount + player->inv->getItem(i)->amount <= Drops::items[player->inv->getItem(i)->id].maxslot){
								stack=1;
								Item* newitem = player->inv->getItem(j);
								newitem->amount += player->inv->getItem(i)->amount;
								amount1 = newitem->amount;
								player->inv->setItem(newitem, j);
								player->inv->deleteItem(i);
							}
							else{
								stack=2;
								Item* newitem1 = player->inv->getItem(j);
								Item* newitem2 = player->inv->getItem(i);
								int amountt = player->inv->getItem(j)->amount;
								newitem1->amount = Drops::items[player->inv->getItem(j)->id].maxslot;
								newitem2->amount -= newitem1->amount - amountt;
								amount2 = newitem1->amount;
								amount1 = newitem2->amount;
								player->inv->setItem(newitem1, j);
								player->inv->setItem(newitem2, i);
							}
						}
						else
							player->inv->setItemPos(j, slot1);
						break;
					}
				}	
				if(stack==0)
					player->inv->setItemPos(i, slot2);	
				break;
			}
		}			
		if(stack==0)
			InventoryPacket::moveItem(player, inv, slot1, slot2);
		else if(stack==1){
			InventoryPacket::moveItemS(player, inv, slot2, amount1);
			InventoryPacket::moveItem(player, inv, slot1, 0);
		}
		else if(stack==2)
			InventoryPacket::moveItemS2(player, inv, slot1, amount1, slot2, amount2);

	}
	player->quests->checkQuests();
}

int Inventory::findSlot(Player* player, int itemid ,char inv, short amount){
	if(inv==1){
		int pos = 1;
		for(unsigned int i=1; i<=100; i++){
			int check=0;
			for(int j=0; j<player->inv->getEquipNum(); j++){
				if(player->inv->getEquipPos(j) == i){
					check = 1;
					if(i==100)
						return 0;
					break;
				}
			}
			if(!check){
				pos = i;
				break;
			}
		}
		return pos;
	}
	else{
		int pos = 1;
		for(unsigned int i=1; i<=100; i++){
			int check=0;
			for(int j=0; j<player->inv->getItemNum(); j++){
				if(((player->inv->getItemPos(j) == i) && !(player->inv->getItem(j)->id == itemid && player->inv->getItem(j)->amount + amount <= Drops::items[itemid].maxslot)) && Drops::items[player->inv->getItem(j)->id].type == inv){
					check = 1;
					if(i==100)
						return 0;
					break;
				}
			}
			if(!check){
				pos = i;
				break;
			}
		}		
		return pos;
	}
	return 0;
}

Equip* Inventory::setEquipStats(Player* player, int equipid){
	EquipInfo ei = Drops::equips[equipid];
	Equip* equip = new Equip;
	equip->id = equipid;
	equip->slots = ei.slots;
	equip->scrolls = 0;
	equip->type = ei.type;
	equip->pos = findSlot(player, equipid, 1, 1);
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
	return equip;
}

void Inventory::addEquip(Player* player, Equip* equip, bool is){
	InventoryPacket::addEquip(player, equip, is);
	player->inv->addEquip(equip);
	player->quests->checkQuests();
}

void Inventory::addItem(Player* player, Item* item, bool is){
	for(int i=0; i<player->inv->getItemNum(); i++){
		if(player->inv->getItem(i)->id == item->id && Drops::items[player->inv->getItem(i)->id].type == item->inv && player->inv->getItem(i)->amount < Drops::items[item->id].maxslot){
			Item* olditem = player->inv->getItem(i);
			if(item->amount + olditem->amount > Drops::items[item->id].maxslot){
				int amount = Drops::items[item->id].maxslot-olditem->amount;
				item->amount-=amount;
				olditem->amount = Drops::items[item->id].maxslot;
				InventoryPacket::addItem(player, olditem, is);
			}
			else{
				item->amount += olditem->amount;
				item->pos = olditem->pos;
				player->inv->deleteItem(i);
				player->inv->addItem(item);
				InventoryPacket::addItem(player, item, is);
				return;
			}
		}
	}
	InventoryPacket::addNewItem(player, item, is);
	player->inv->addItem(item);
}

void Inventory::buyItem(Player* player, unsigned char* packet){
	int item = getInt(packet+3);
	short howmany = getShort(packet+7);
	bool isequip=false;
	if(Drops::equips.find(item) != Drops::equips.end())
		isequip = 1;
	if(isequip){
		Equip* equip = setEquipStats(player, item);
		addEquip(player, equip);
		player->inv->setMesos(player->inv->getMesos() - Shops::getPrice(player, item));
		InventoryPacket::bought(player);
	}
	else{
		if(Drops::items.find(item) == Drops::items.end())
			return;
		char type = Drops::items[item].type;
		Item* newitem = new Item;
		newitem->amount = howmany;
		newitem->id = item;
		newitem->inv = type;
		newitem->pos = findSlot(player, item , type, howmany);
		addItem(player, newitem);
		player->inv->setMesos(player->inv->getMesos() - Shops::getPrice(player, item)*howmany);
		InventoryPacket::bought(player);
	}
}

void Inventory::addNewItem(Player* player, int item, int howmany){
	bool isequip=false;
	if(Drops::equips.find(item) != Drops::equips.end())
		isequip = 1;
	if(isequip){
		Equip* equip = setEquipStats(player, item);
		addEquip(player, equip);
	}
	else{
		if(Drops::items.find(item) == Drops::items.end())
			return;
		char type = Drops::items[item].type;
		Item* newitem = new Item;
		newitem->amount = howmany;
		newitem->id = item;
		newitem->inv = type;
		newitem->pos = findSlot(player, item , type, howmany);
		addItem(player, newitem);
	}
	player->quests->checkQuests();
}

void Inventory::takeItem(Player* player, int item, int howmany){
	for(int i=0; i<player->inv->getItemNum(); i++)
		if(player->inv->getItem(i)->id == item){
			if(player->inv->getItem(i)->amount>=howmany){
				Item* item = player->inv->getItem(i);
				item->amount-=howmany;
				howmany=0;
				if(item->amount == 0){
					InventoryPacket::moveItem(player, item->inv, item->pos, 0);
					player->inv->deleteItem(i);
				}
				else
					InventoryPacket::moveItemS(player, item->inv, item->pos, item->amount);
				break;
			}
			else{
				Item* item = player->inv->getItem(i);
				howmany-=item->amount;
				item->amount=0;
				InventoryPacket::moveItem(player, item->inv, item->pos, 0);
				player->inv->deleteItem(i);
			}
		}
	player->quests->checkQuests();
}

void Inventory::takeItemSlot(Player* player, short slot){
	for(int i=0; i<player->inv->getItemNum(); i++)
		if(player->inv->getItem(i)->pos == slot && player->inv->getItem(i)->inv == 2){
			Item* item = player->inv->getItem(i);
			item->amount-=1;
			if(item->amount == 0){
				InventoryPacket::moveItem(player, item->inv, item->pos, 0);
				player->inv->deleteItem(i);
			}
			else
				InventoryPacket::moveItemS(player, item->inv, item->pos, item->amount);
			break;
		}
	player->quests->checkQuests();
}

void Inventory::useItem(Player *player, unsigned char *packet){
	short slot = getShort(packet+4);
	int itemid = getInt(packet+6);
	takeItemSlot(player, slot);
	if(Drops::consumes[itemid].hp>0){
		player->setHP(player->getHP()+Drops::consumes[itemid].hp);
	}
	if(Drops::consumes[itemid].mp>0){ 
		player->setMP(player->getMP()+Drops::consumes[itemid].mp);
	}
	if(Drops::consumes[itemid].hpr>0){
		player->setHP(player->getHP()+Drops::consumes[itemid].hpr*player->getMHP()/100);
	}
	if(Drops::consumes[itemid].mpr>0){
		player->setMP(player->getMP()+Drops::consumes[itemid].mpr*player->getMMP()/100);
	}

	
}

void Inventory::useChair(Player* player, unsigned char* packet){
	int chairid = getInt(packet);
	InventoryPacket::sitChair(player, Maps::info[player->getMap()].Players, chairid);
}

void Inventory::stopChair(Player* player, unsigned char* packet){
	InventoryPacket::stopChair(player, Maps::info[player->getMap()].Players);
}