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
#include "PlayerHandler.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "PacketCreator.h"
#include "Player.h"
#include "Map.h"
#include "MapDrops.h"
#include "DataProvider.h"
#include "PlayerInventories.h"
#include "Channel.h"
#include "Effect.h"
#include "Maps.h"
#include "Pet.h"
#include "Inventory.h"
#include "Item.h"
#include "ObjectMoving.h"
#include "PlayerSkills.h"
#include "Skill.h"
#include "Equip.h"
#include "ItemData.h"
#include "EquipData.h"
#include <algorithm>
#include "ItemEffectData.h"
#include "PlayerBuffs.h"
#include "Tools.h"
using namespace Tools;

void PlayerHandler::cancelChairHandle(PacketReader& packet){
	player->setChair(0);
	player->send(PacketCreator().cancelChair());
	player->getMap()->send(PacketCreator().useChair(player->getID(), 0), player); 
}
void PlayerHandler::useChairHandle(PacketReader& packet){
	int chairid = packet.readInt();
	Item* item = player->getInventories()->getItemByID(chairid);
	if(item == NULL || MAJOR_TYPE(chairid) != 301) // not a chair
		return;
	player->setChair(chairid);
	player->getMap()->send(PacketCreator().useChair(player->getID(), chairid), player); 
}
void PlayerHandler::useItemEffectHandle(PacketReader& packet){
	int effect = packet.readInt();
	Item* item = player->getInventories()->getItemByID(effect);
	if((item == NULL || MAJOR_TYPE(effect) != 501) && effect != 0) 
		return;
	player->setItemEffect(effect);
	player->getMap()->send(PacketCreator().useItemEffect(player->getID(), effect), player); 

}
void PlayerHandler::useCashItemHandle(PacketReader& packet){
	short slot = packet.readShort();
	int itemid = packet.readInt();
	Item* item = player->getInventories()->getItemBySlot(CASH, slot);
	if(item == NULL || item->getID() != itemid){
		return;
	}
	switch(MAJOR_TYPE(itemid)){
		//case 503: break; //TODO: Shops
		//case 504: break; //TODO: VIP Rock
		case 505:
			{
				int to = packet.readInt();
				int from = packet.readInt();
				if(itemid%10 == 0){
					Values values;
					Value v = player->removeStat(from);
					if(v.getType() == 0)
						return;
					values.add(v);
					v = player->addStat(to);
					if(v.getType() == 0)
						return;
					values.add(v);
					player->send(PacketCreator().updateStats(values, true));
				}
				else{
					int job = itemid%10;
					if(SKILL_JOB_N(to) != job || SKILL_JOB_N(from) > job)
						return;
					Skill* froms = player->getSkills()->getSkill(from);
					Skill* tos =  player->getSkills()->getSkill(to);
					if(froms->getLevel() == 0 || tos->getLevel () == tos->getMasterLevel())
						return;
					froms->setLevel(froms->getLevel()-1);
					tos->setLevel(tos->getLevel()+1);
					vector <Skill*> skills;
					skills.push_back(froms);
					skills.push_back(tos);
					player->send(PacketCreator().updateSkill(skills));

				}
				break;
			}
		case 506:
			{
				switch(itemid%10){
					case 0:
						{
							short pos = packet.readShort();
							Equip* equip = (Equip*)player->getInventories()->getItemBySlot(EQUIPPED, pos);
							if(equip == NULL || equip->getOwner() != "")
								return;
							equip->setOwner(player->getName());
							player->send(PacketCreator().newItem(equip, true));
							break;
						}
					/*
					case 1:
						{	 // How to unlock item??
							packet.show();
							int inv = packet.readInt();
							int pos = packet.readInt();
							Item* on = player->getInventories()->getItemBySlot(inv, pos);
							if(on == NULL)
								return;
							on->setLocked(!on->getLocked());
							player->send(PacketCreator().newItem(on, true));
							break;
						}
					case 2: break;// egg?				
					*/
					default: player->send(PacketCreator().enableAction());
				}
				break;
			} 
		//case 507: break;// TODO: megaphone
		//case 508: break; // Old thing 0.o
			/*
		case 509:
			{	
				string receiver = packet.readString();
				string note = packet.readString();
				// TODO: Get note packet 0.o
				break;
			}
		case 510: 
			{
				player->getMap()->send(PacketCreator().changeSound("Jukebox/Congratulation")); // need to add timer
			}*/
		case 530:
			{
				Effect* effect = Effect::getEffect(itemid);
				if(effect != NULL)
					effect->use(player);
				else
					player->send(PacketCreator().enableAction());
			}
		default:{ player->send(PacketCreator().enableAction()); return; };
	}
	player->getInventories()->removeItemBySlot(CASH, slot, 1);
}
void PlayerHandler::moveItemHandle(PacketReader& packet){
	packet.readInt();
	char inv1 = packet.read();
	char inv2 = inv1;
	short slot1 = packet.readShort();
	if(slot1 < 0)
		inv1 = 0;
	short slot2 = packet.readShort();
	if(slot2 < 0)
		inv2 = 0;
	Item* item1 = player->getInventories()->getItemBySlot(inv1, slot1);
	Item* item2 = player->getInventories()->getItemBySlot(inv2, slot2);
	short amount= packet.readShort();
	if(item1 == NULL || (amount == 0 && !IS_STAR(item1->getID())))
		return;
	if(slot2 == 0){ // Drop
		if((amount > item1->getAmount() || amount < 1) && !IS_STAR(item1->getID()))
			return;
		if(IS_STAR(item1->getID()))
			amount = item1->getAmount();
		if(amount!=item1->getAmount()){
			Item* item = new Item(item1->getID(), amount);
			item1->setAmount(item1->getAmount()-amount);
			player->getMap()->getDrops()->dropFromObject(item, player->getPosition(), player, 0, player);
			player->send(PacketCreator().updateSlot(item1, true));
		}
		else{
			player->getMap()->getDrops()->dropFromObject(item1, player->getPosition(), player,0,player);
			player->getInventories()->getInventory(inv1)->removeItem(slot1, true, false, false);
			player->send(PacketCreator().moveItem((inv1 == 0 || inv2 == 0) ? 1 : inv1, slot1, slot2, (inv1 == 0 || inv2 == 0)));
		}
	}
	else{
		if(item2 == NULL){ // move to empty pos
			item1->setSlot(slot2);
			player->getInventories()->getInventory(inv1)->removeItem(slot1, false, false, false);
			player->getInventories()->getInventory(inv2)->addItem(item1, false, false, false);
			player->send(PacketCreator().moveItem((inv1 == 0 || inv2 == 0) ? 1 : inv1, slot1, slot2, (inv1 == 0 || inv2 == 0)));
		}
		else if(item2->getID() != item1->getID() || inv1 <= 1 || inv2 <= 1 || IS_STAR(item1->getID()) || IS_STAR(item2->getID())){ // switch positions
			item1->setSlot(slot2);
			item2->setSlot(slot1);
			player->getInventories()->getInventory(inv1)->removeItem(slot1, false, false, false);
			player->getInventories()->getInventory(inv2)->removeItem(slot2, false, false, false);
			player->getInventories()->getInventory(inv2)->addItem(item1, false, false, false);
			player->getInventories()->getInventory(inv1)->addItem(item2, false, false, false);
			player->send(PacketCreator().moveItem((inv1 == 0 || inv2 == 0) ? 1 : inv1, slot1, slot2, (inv1 == 0 || inv2 == 0)));
		}
		else{ // merge
			if(item1->getAmount() + item2->getAmount() <= DataProvider::getInstance()->getItemMaxPerSlot(item1->getID())){ 
				item2->setAmount(item1->getAmount()+item2->getAmount());
				player->send(PacketCreator().moveItemMerge(inv2, slot1, slot2, item2->getAmount()));
				player->getInventories()->getInventory(inv1)->removeItem(slot1, true, true, false);
			}
			else{
				int change = DataProvider::getInstance()->getItemMaxPerSlot(item1->getID()) - item2->getAmount();
				item2->setAmount(DataProvider::getInstance()->getItemMaxPerSlot(item1->getID()));
				item1->setAmount(item1->getAmount()-change);
				player->send(PacketCreator().moveItemMergeTwo(inv1, slot1, item1->getAmount(), slot2, item2->getAmount()));
			}
		}
	}
	if(inv1 == 0 || inv2 == 0)
		player->getMap()->send(PacketCreator().updatePlayer(player), player);
}
void PlayerHandler::useItemHandle(PacketReader& packet){
	packet.readInt();
	short slot = packet.readShort();
	int itemid = packet.readInt();
	Item* item = player->getInventories()->getItemBySlot(USE, slot);
	if(item == NULL || item->getID() != itemid){
		player->send(PacketCreator().enableAction());
		return;
	}
	player->getInventories()->removeItemBySlot(USE, slot, 1);
	Effect* effect = Effect::getEffect(itemid);
	if(effect != NULL)
		effect->use(player);	
}
void PlayerHandler::useReturnScrollHandle(PacketReader& packet){
	packet.readInt();
	short slot = packet.readShort();
	int itemid = packet.readInt();
	Item* item = player->getInventories()->getItemBySlot(USE, slot);
	if(item == NULL || item->getID() != itemid || MAJOR_TYPE(itemid) != 203){
		player->send(PacketCreator().enableAction());
		return;
	}
	player->getInventories()->removeItemBySlot(USE, slot, 1);
	int tomapid = DataProvider::getInstance()->getItemMoveTo(itemid);
	if(tomapid == 999999999){	
		int mapid = player->getMap()->getReturnMap();
		if(mapid == 999999999){
			player->changeMap(player->getMap());
		}
		else{
			Map* map = player->getChannel()->getMaps()->getMap(mapid);
			if(map == NULL){
				player->changeMap(player->getMap());
			}
			else{
				player->changeMap(map);
			}
		}
	}
	else{
		Map* map = player->getChannel()->getMaps()->getMap(tomapid);
		if(map != NULL)
			player->changeMap(map);
	}
}
int getRandomStat(){
	return random(1,5)*((random(2) == 1) ? 1 : -1);
}
void PlayerHandler::useScrollHandle(PacketReader& packet){
	packet.readInt();
	short slot = packet.readShort();
	short eslot = packet.readShort();
	bool wscroll = packet.readShort() == 2;
	Item* scroll = player->getInventories()->getItemBySlot(USE, slot);
	Equip* equip = (Equip*)player->getInventories()->getItemBySlot(EQUIPPED, eslot);
	if(scroll == NULL || equip == NULL || MAJOR_TYPE(scroll->getID()) != 204 || equip->getSlots() == 0 || (wscroll && player->getInventories()->getItemByID(2340000) == NULL)){
		player->send(PacketCreator().enableAction());
		return;
	}
	if(wscroll){
		player->getInventories()->getInventory(USE)->removeItem(2340000, (int)1);
	}
	ItemData* item = DataProvider::getInstance()->getItemData(scroll->getID());
	if(item == NULL)
		return;
	bool success = false, cursed = false;
	ItemEffectData* effect = item->getEffectData();
	if(effect->getRecover() && equip->getSlots() >= DataProvider::getInstance()->getEquipData(equip->getID())->getSlots()){
		player->send(PacketCreator().enableAction());
		return;
	}
	if(random(100) < effect->getSuccess()){
		success = true;
		equip->setStr(equip->getStr()+effect->getStr());
		equip->setDex(equip->getDex()+effect->getDex());
		equip->setInt(equip->getInt()+effect->getInt());
		equip->setLuk(equip->getLuk()+effect->getLuk());
		equip->setHP(equip->getHP()+effect->getHP());
		equip->setMP(equip->getMP()+effect->getMP());
		equip->setWAtk(equip->getWAtk()+effect->getWAtk());
		equip->setMAtk(equip->getMAtk()+effect->getMAtk());
		equip->setWDef(equip->getWDef()+effect->getWDef());
		equip->setMDef(equip->getMDef()+effect->getMDef());
		equip->setAcc(equip->getAcc()+effect->getAcc());
		equip->setAvo(equip->getAvo()+effect->getAvo());
		equip->setHand(equip->getHand()+effect->getHand());
		equip->setJump(equip->getJump()+effect->getJump());
		equip->setSpeed(equip->getSpeed()+effect->getSpeed());
		if(effect->getRecover()){
			equip->setSlots(equip->getSlots()+1);
		}
		else{
			equip->setScrolls(equip->getScrolls()+1);
		}
		if(effect->getRandStat()){
			if(equip->getStr() > 0)
				equip->setStr(equip->getStr()+getRandomStat());
			if(equip->getDex() > 0)
				equip->setDex(equip->getDex()+getRandomStat());
			if(equip->getInt() > 0)
				equip->setInt(equip->getInt()+getRandomStat());
			if(equip->getLuk() > 0)
				equip->setLuk(equip->getLuk()+getRandomStat());
			if(equip->getHP() > 0)
				equip->setHP(equip->getHP()+getRandomStat());
			if(equip->getMP() > 0)
				equip->setMP(equip->getMP()+getRandomStat());
			if(equip->getWAtk() > 0)
				equip->setWAtk(equip->getWAtk()+getRandomStat());
			if(equip->getStr() > 0)
				equip->setMAtk(equip->getMAtk()+getRandomStat());
			if(equip->getStr() > 0)
				equip->setWDef(equip->getWDef()+getRandomStat());
			if(equip->getMDef() > 0)
				equip->setMDef(equip->getMDef()+getRandomStat());
			if(equip->getAcc() > 0)
				equip->setAcc(equip->getAcc()+getRandomStat());
			if(equip->getAvo() > 0)
				equip->setAvo(equip->getAvo()+getRandomStat());
			if(equip->getHand() > 0)
				equip->setHand(equip->getHand()+getRandomStat());
			if(equip->getJump() > 0)
				equip->setJump(equip->getJump()+getRandomStat());
			if(equip->getSpeed() > 0)
				equip->setSpeed(equip->getSpeed()+getRandomStat());
		}
	}
	else{
		if(random(100) < effect->getCursed() || effect->getRandStat()){
			cursed = true;
		}
	}
	if(!((wscroll && !success) || effect->getRecover()))
		equip->setSlots(equip->getSlots()-1);
	player->getMap()->send(PacketCreator().useScroll(player->getID(), success, cursed));
	player->send(PacketCreator().scrollItem(scroll, equip, cursed));
	player->getInventories()->removeItemBySlot(USE, slot, 1, false);
	if(cursed){
		player->getMap()->send(PacketCreator().updatePlayer(player), player);
		player->getInventories()->removeItemBySlot(EQUIPPED, eslot, 1, false);
	}
}
void PlayerHandler::useSummonBugHandle(PacketReader& packet){
}

void PlayerHandler::cancelItemBuffHandle(PacketReader& packet){
	int effect = packet.readInt();
	player->getBuffs()->cancelBuff(effect);	
}

void PlayerHandler::usePetFoodHandle(PacketReader& packet){
	packet.readInt();
	short slot = packet.readShort();
	int itemid = packet.readInt();
	Item* item = player->getInventories()->getItemBySlot(USE, slot);
	if(item == NULL || MAJOR_TYPE(itemid) != 212 || item->getID() != itemid) 
		return;
	vector <Pet*>* pets = player->getPets();
	if(!pets->empty())
		player->getInventories()->removeItemBySlot(USE, slot, 1);
	else{
		player->send(PacketCreator().emptyInventoryAction());
		return;
	}
	ItemData* data = DataProvider::getInstance()->getItemData(itemid);
	if(data == NULL)
		return;
	ItemEffectData* effect = data->getEffectData();
	for(int i=0; i<(int)pets->size(); i++){
		bool closeness = (*pets)[i]->getFullness() < 50;
		(*pets)[i]->addFullness(effect->getFullness());
		if(closeness && random(2))
			(*pets)[i]->addCloseness(1, player);
		else
			player->send(PacketCreator().updatePet((*pets)[i]));
	}
}

void PlayerHandler::autoArrangementHandle(PacketReader& packet){
	// I need to get the packet ><
	/*
	packet.show();
	packet.readInt();
	int inv = packet.read();
	hash_map <int, Item*>* hitems = player->getInventories()->getInventory(inv)->getItems();
	vector <Item*> items;
	for(hash_map<int, Item*>::iterator iter = hitems->begin(); iter != hitems->end(); iter++){
		items.push_back(iter->second);	
	}
	sort<vector <Item*>::iterator, CompareItems>(items.begin(),items.end(), CompareItems());
	//TODO: find the packets
	bool check = true;
	for(int i=0; i<(int)items.size(); i++)
		if(items[i]->getSlot() != i+1) check = false;
	if(check){ // TODO: stacking
	}
	else{
		for(int i=0; i<(int)items.size(); i++){	
			Item* item1 = items[i];
			Item* item2 = player->getInventories()->getItemBySlot(inv, i+1);
			if(item1 == item2) continue;
			short slot1 = items[i]->getSlot();
			short slot2 = i+1;
			if(item2 == NULL){
				item1->setSlot(slot2);
				player->getInventories()->getInventory(inv)->removeItem(slot1, false, false, false);
				player->getInventories()->getInventory(inv)->addItem(item1, false, false, false);
			}
			else {
				item1->setSlot(slot2);
				item2->setSlot(slot1);
				player->getInventories()->getInventory(inv)->removeItem(slot1, false, false, false);
				player->getInventories()->getInventory(inv)->removeItem(slot2, false, false, false);
				player->getInventories()->getInventory(inv)->addItem(item1, false, false, false);
				player->getInventories()->getInventory(inv)->addItem(item2, false, false, false);
			}
			player->send(PacketCreator().moveItem(inv, slot1, slot2, inv));
		}
	}
	*/
}