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

#include "PacketCreator.h"
#include "PacketWriter.h"
#include "Player.h"
#include "Tools.h"
#include "Map.h"
#include "Item.h"
#include "PlayerInventories.h"
#include "Inventory.h"
#include "ObjectMoving.h"
#include "Channel.h"
#include "Skill.h"
#include "PlayerSkills.h"
#include "Quest.h"
#include "PlayerQuests.h"
#include "Key.h"
#include "PlayerKeys.h"
#include "DataProvider.h"
#include "PlayerBuffs.h"
#include "Pet.h"
#include "Effect.h"

#include <hash_map>
using namespace std;
using namespace stdext;
using namespace Tools;


PacketWriter* PacketCreator::updatePlayer(Player* player){
	pw.writeShort(UPDATE_LOOK);

	pw.writeInt(player->getID());
	pw.write(1);
	playerShow(player);
	pw.writeInt(0);
	pw.writeShort(0);

	return &pw;
}
PacketWriter* PacketCreator::useScroll(int playerid, bool success, bool cursed){
	pw.writeShort(SCROLL_EFFECT);

	pw.writeInt(playerid);
	pw.write(success);
	pw.write(cursed);
	pw.writeShort(0);

	return &pw;
}
PacketWriter* PacketCreator::useItemEffect(int playerid, int itemid){
	pw.writeShort(ITEM_EFFECT);

	pw.writeInt(playerid);
	pw.writeInt(itemid);

	return &pw;
}
PacketWriter* PacketCreator::useChair(int playerid, int chairid){
	pw.writeShort(SHOW_CHAIR);

	pw.writeInt(playerid);
	pw.writeInt(chairid);

	return &pw;
}
PacketWriter* PacketCreator::cancelChair(){
	pw.writeShort(CANCEL_CHAIR);

	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::showEffect(int playerid, char effect, int what, bool buff){
	pw.writeShort(SHOW_PLAYER_EFFECT);

	pw.writeInt(playerid);
	pw.write(effect);
	
	if(buff){
		pw.writeInt(what);
		pw.write(1);
	}
	else{
		pw.writeShort(0);
	}

	return &pw;
}
PacketWriter* PacketCreator::showPlayerEffect(char effect, int what, bool buff){
	pw.writeShort(ITEM_GAIN_CHAT);

	pw.write(effect);
	if(buff){
		pw.writeInt(what);
		pw.write(1);
	}
	else{
		pw.writeShort(what);
	}

	return &pw;
}

PacketWriter* PacketCreator::showBuffEffect(int playerid, char effect, int source){
	return showEffect(playerid, effect, source, true);
}
PacketWriter* PacketCreator::showPlayerBuffEffect(char effect, int source){
	return showPlayerEffect(effect, source, true);
}
PacketWriter* PacketCreator::showPlayerBuff(Values* values, int buffid, int time){
	pw.writeShort(SHOW_PLAYER_BUFF);

	pw.writeLong(getStatsType(values));
	values->sort();
	vector <Value>* v = values->getValues();
	for(int i=0; i<(int)v->size(); i++){
		pw.writeShort((*v)[i].getValue());
		pw.writeInt(buffid);
		pw.writeInt(time);
	}
	pw.writeShort(0);
	pw.write(0);

	pw.show();

	return &pw;
}
PacketWriter* PacketCreator::showBuff(int playerid, Values* values){
	pw.writeShort(SHOW_BUFF);

	pw.writeInt(playerid);

	pw.writeLong(getStatsType(values));
	values->sort();
	vector <Value>* v = values->getValues();
	for(int i=0; i<(int)v->size(); i++){
		pw.writeShort((*v)[i].getValue());
	}
	pw.writeShort(0);

	return &pw;
}
PacketWriter* PacketCreator::cancelPlayerBuff(Values* values){
	pw.writeShort(CANCEL_PLAYER_BUFF);

	pw.writeLong(getStatsType(values));
	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::cancelBuff(int playerid, Values* values){
	pw.writeShort(CANCEL_BUFF);

	pw.writeInt(playerid);
	pw.writeLong(getStatsType(values));

	return &pw;
}
void PacketCreator::playerInfo(Player* player){
	pw.writeInt(player->getID());
	pw.writeString(player->getName(), 13);
	pw.write(player->getGender());
	pw.write(player->getSkin());
	pw.writeInt(player->getFace());
	pw.writeInt(player->getHair());
	pw.writeLong(0);
	pw.writeLong(0);
	pw.writeLong(0);
	pw.write(player->getLevel());
	pw.writeShort(player->getJob());
	pw.writeShort(player->getStr());
	pw.writeShort(player->getDex());
	pw.writeShort(player->getInt());
	pw.writeShort(player->getLuk());
	pw.writeShort(player->getHP());
	pw.writeShort(player->getMaxHP());
	pw.writeShort(player->getMP());
	pw.writeShort(player->getMaxMP());
	pw.writeShort(player->getAP());
	pw.writeShort(player->getSP());
	pw.writeInt(player->getExp());
	pw.writeShort(player->getFame());
	pw.writeInt(player->getMap()->getID());
	pw.write(player->getMappos());
}
void PacketCreator::playerShow(Player* player, bool smega){
	pw.write(player->getGender());
	pw.write(player->getSkin());
	pw.writeInt(player->getFace());
	pw.write(!smega);
	pw.writeInt(player->getHair());
	
	Inventory* i = player->getInventories()->getInventory(EQUIPPED);
	hash_map<char, int> visible;
	hash_map<char, int> invisible;;
	vector<Item*> items = i->getItems();
	Item* item = NULL;
	for(int i=0; i<(int)items.size() && (item = items[i]) != NULL; i++){
		if(item->getSlot() != -111){ // For cash weapons
			char epos = item->getSlot()*-1;
			if(epos < 100){
				if(visible.find(epos) == visible.end())
					visible[epos] = item->getID();
				else
					invisible[epos] = item->getID();
			}
			else { // cash
				epos-=100;
				if(visible.find(epos) != visible.end())
					invisible[epos] = visible[epos];
				visible[epos] = item->getID();
			}
		}
	}
	// Show visible items
	for (hash_map<char,int>::iterator iter = visible.begin(); iter != visible.end(); iter++){
		pw.write(iter->first);
		pw.writeInt(iter->second);
	}
	// Show unvisible items
	pw.write(-1);
	for (hash_map<char,int>::iterator iter = invisible.begin(); iter != invisible.end(); iter++){
		pw.write(iter->first);
		pw.writeInt(iter->second);
	}
	// Show cash weapon
	pw.write(-1);
	Item* w = i->getItemBySlot(-111);
	pw.writeInt(w ? w->getID() : 0);
	if(player->getPet() != NULL)
		pw.writeInt(player->getPet()->getItemID());
	else
		pw.writeInt(0);
	pw.writeInt(0); // Pet2?
	pw.writeInt(0); // Pet3?
}
PacketWriter* PacketCreator::makeApple(){
	pw.writeShort(MAKE_APPLE);

	return &pw;
}
PacketWriter* PacketCreator::connectionData(Player* player){
	pw.writeShort(CHANGE_MAP);

	pw.writeInt(player->getChannel()->getID());
	pw.write(1);
	pw.write(1);
	//0.57
	//pw.writeShort(0);
	//
	pw.writeInt(randomInt());	
	pw.writeInt(randomInt());	
	pw.writeInt(randomInt());	
	//0.57
	pw.writeShort(-1);
	//pw.writeInt(-1);
	//pw.writeInt(-1);
	//
	playerInfo(player);
	pw.write(0x14);
	pw.writeInt(player->getMesos());
	for(int i=1; i<=5; i++)
		pw.write(player->getInventories()->getInventory(i)->getSlots());
	for(int i=0; i<=5; i++){
		Inventory* in = player->getInventories()->getInventory(i);
		vector<Item*> items = in->getItems();
		Item* item = NULL;
		if(i == 0){
			for(int j=0; j<(int)items.size() && (item = items[j]) != NULL && !DataProvider::getInstance()->isItemCash(items[j]->getID()); j++){
				itemInfo(item);
			}
			pw.write(0);
			for(int j=0; j<(int)items.size() && (item = items[j]) != NULL && DataProvider::getInstance()->isItemCash(items[j]->getID()); j++){
				itemInfo(item);
			}
		}
		else{
			for(int j=0; j<(int)items.size() && (item = items[j]) != NULL; j++){
				itemInfo(item);
			}
		}
		pw.write(0);
	}
	vector <Skill*> skills = player->getSkills()->getSkillsInfo();
	pw.writeShort(skills.size());
	Skill* skill= NULL;
	for(int i=0; i<(int)skills.size() && (skill = skills[i]) != NULL; i++){
		pw.writeInt(skill->getID());
		pw.writeInt(skill->getLevel());
		if(FOURTH_JOB(skill->getID())){
			pw.writeInt(skill->getMasterLevel());
		}
	}
	pw.writeShort(0);
	vector <Quest*> p = player->getQuests()->getQuestsInProgress();
	vector <Quest*> c = player->getQuests()->getCompletedQuests();
	pw.writeShort(p.size());
	Quest* quest=NULL;
	for(int i=0; i<(int)p.size() && (quest = p[i]) != NULL; i++){
		pw.writeInt(quest->getID());
	}
	pw.writeShort(c.size());
	for(int i=0; i<(int)c.size() && (quest = c[i]) != NULL; i++){
		pw.writeShort(quest->getID());
		pw.writeLong(quest->getCompleteTime());
	}
	pw.writeLong(0);
	for(int i=0; i<15; i++)
		pw.writeBytes("FFC99A3B");
	//0.57
	//pw.writeInt(0);
	//
	pw.writeLong(getLongTime());

	return &pw;
}
PacketWriter* PacketCreator::showKeys(PlayerKeys* keys){
	pw.writeShort(SHOW_KEYS);

	pw.write(0);
	Key* key;
	for(int i=0; i<90; i++){
		key = keys->getKey(i);
		if(key != NULL){
			pw.write(key->getType());
			pw.writeInt(key->getAction());
		}
		else{
			pw.write(0);
			pw.writeInt(0);
		}
	}

	return &pw;
}
PacketWriter* PacketCreator::showMoving(int playerid, ObjectMoving* moving){
	pw.writeShort(PLAYER_MOVE);
	
	pw.writeInt(playerid);
	pw.writeInt(0);
	pw.write(moving->getPacket()->getBytes(), moving->getPacket()->getLength());

	return &pw;
}
PacketWriter* PacketCreator::faceExpression(int playerid, int face){
	pw.writeShort(FACE_EEXPRESSION);

	pw.writeInt(playerid);
	pw.writeInt(face);

	return &pw;
}
PacketWriter* PacketCreator::showChatMassage(int playerid, string msg, bool gm){
	pw.writeShort(CHAT_MASSAGE);

	pw.writeInt(playerid);
	pw.write(gm);
	pw.writeString(msg);

	return &pw;
}
PacketWriter* PacketCreator::damagePlayer(int playerid, int skill, int dmg, int obj){
	pw.writeShort(DAMAGE_PLAYER);

	pw.writeInt(playerid);
	pw.write(skill);
	pw.writeInt(0);
	if(skill == -2)
		pw.writeInt(dmg);
	else
		pw.writeInt(obj);
	pw.writeShort(1);
	pw.write(0);
	pw.writeInt(dmg);

	return &pw;
}
PacketWriter* PacketCreator::showMassage(string msg, char type, int channel, bool server){
	pw.writeShort(SHOW_MASSAGE);
	
	pw.write(type);
	if(server){
		pw.write(1);
	}
	pw.writeString(msg);
	if(type == 3){
		pw.write(channel);
		pw.write(0);
	}

	return &pw;
}
PacketWriter* PacketCreator::showInfo(Player* player){
	pw.writeShort(PLAYER_INFO);
	
	pw.writeInt(player->getID());
	pw.write(player->getLevel());
	pw.writeShort(player->getJob());
	pw.writeShort(player->getFame());
	pw.write(0); // Married
	pw.writeString("-"); // Guild Name
	if(player->getPet() != NULL){
		pw.write(1);
		Pet* pet = player->getPet();
		pw.writeInt(pet->getItemID());
		pw.writeString(pet->getName());
		pw.write(pet->getLevel());
		pw.writeShort(pet->getCloseness());
		pw.write(pet->getFullness());
		pw.writeShort(0);
		pw.write(0);
		pw.writeInt(0);
	}
	else{
		pw.write(0); 
	}
	/*
		Pet:
		ID int
		Name string
		Level byte
		Closeness short
		Fullness byte
		Unk int (48?)
		
	*/
	bool taming = player->getBuffs()->isBuffActive(Effect::MONSTER_RIDING);
	pw.write(taming); 
	if(taming){
		pw.writeInt(0); // Level
		pw.writeInt(0); // EXP
		pw.writeInt(0); // Tiredness

	}
	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerReplay(string name, bool success){
	pw.writeShort(FIND_PLAYER);

	pw.write(0xA);
	pw.writeString(name);
	pw.write(success);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerMap(string name, int map, int channel){
	pw.writeShort(FIND_PLAYER);

	pw.write(0x9);
	pw.writeString(name);
	pw.write(1);
	pw.writeInt(map);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerChannel(string name, int map, int channel){
	pw.writeShort(FIND_PLAYER);

	pw.write(0x9);
	pw.writeString(name);
	pw.write(3);
	pw.writeInt(channel);

	return &pw;
}
PacketWriter* PacketCreator::whisperPlayer(string name, string msg, int channel){
	pw.writeShort(FIND_PLAYER);

	pw.write(0x12);
	pw.writeString(name);
	pw.writeShort(channel);
	pw.writeString(msg);

	return &pw;
}
PacketWriter* PacketCreator::changeChannel(char channelid, short port){
	pw.writeShort(CHANGE_CHANNEL);
		
	pw.write(channelid);
	// TODO
	pw.write(127);
	pw.write(0);
	pw.write(0);
	pw.write(1);
	pw.writeShort(port);
	return &pw;
}
