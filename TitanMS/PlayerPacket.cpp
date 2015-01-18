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
#include "Worlds.h"
#include "Pet.h"
#include "Effect.h"

#include <hash_map>
using namespace std;
using namespace stdext;
using namespace Tools;

PacketWriter* PacketCreator::ping(){
	pw.writeShort(PING);

	return &pw;
}

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
		pw.writeShort(what);
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

	pw.writeLong(0);

	__int64 type = getStatsType(values);
	pw.writeInt((int)(type/0x100000000));	
	pw.writeInt((int)(type%0x100000000));

	values->sort();
	vector <Value>* v = values->getValues();
	for(int i=0; i<(int)v->size(); i++){
		pw.writeShort((*v)[i].getValue());
		pw.writeInt(buffid);
		pw.writeInt(time);
	}
	pw.writeShort(0);
	pw.writeInt(0);

	pw.show();

	return &pw;
}
PacketWriter* PacketCreator::showBuff(int playerid, Values* values){
	pw.writeShort(SHOW_BUFF);

	pw.writeInt(playerid);
	pw.writeLong(0);

	__int64 type = getStatsType(values);
	pw.writeInt((int)(type/0x100000000));	
	pw.writeInt((int)(type%0x100000000));	

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

	pw.writeLong(0);

	__int64 type = getStatsType(values);
	pw.writeInt((int)(type/0x100000000));	
	pw.writeInt((int)(type%0x100000000));	

	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::cancelBuff(int playerid, Values* values){
	pw.writeShort(CANCEL_BUFF);

	pw.writeInt(playerid);
	pw.writeLong(0);

	__int64 type = getStatsType(values);
	pw.writeInt((int)(type/0x100000000));	
	pw.writeInt((int)(type%0x100000000));

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
	hash_map<int, Item*>* items = i->getItems();
	for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
		Item* item = iter->second;
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
	vector <Pet*>* pets = player->getPets();
	for(int i=0; i<(int)pets->size(); i++)
		pw.writeInt((*pets)[i]->getItemID());
	for(int i=(int)pets->size(); i<3; i++)
		pw.writeInt(0);
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
	pw.writeShort(0);
	pw.writeInt(randomInt());	
	pw.writeInt(randomInt());	
	pw.writeInt(randomInt());	
	pw.writeInt(-1);
	pw.writeInt(-1);
	playerInfo(player);
	pw.write(0x14);
	pw.writeInt(player->getMesos());
	for(int i=1; i<=5; i++)
		pw.write(player->getInventories()->getInventory(i)->getSlots());
	for(int i=0; i<=5; i++){
		Inventory* in = player->getInventories()->getInventory(i);
		hash_map<int, Item*>* items = in->getItems();
		if(i == EQUIPPED){
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				Item* item = iter->second;
				if(!DataProvider::getInstance()->isItemCash(item->getID()))
					itemInfo(item);
			}
			pw.write(0);
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				Item* item = iter->second;
				if(DataProvider::getInstance()->isItemCash(item->getID()))
					itemInfo(item);
			}
		}
		else{
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				itemInfo(iter->second);
			}
		}
		pw.write(0);
	}
	hash_map <int, Skill*>* skills = player->getSkills()->getSkillsInfo();
	pw.writeShort(skills->size());
	for(hash_map<int, Skill*>::iterator iter = skills->begin(); iter!=skills->end(); iter++){
		Skill* skill = iter->second;
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
	// The Teleport Rock maps
	for(int i=0; i<5; i++)
		pw.writeInt(999999999);
	// VIP Teleport Rock maps
	for(int i=0; i<10; i++)
		pw.writeInt(999999999);
	pw.writeInt(1);
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
PacketWriter* PacketCreator::showMoving(int playerid, ObjectMoving& moving){
	pw.writeShort(PLAYER_MOVE);
	
	pw.writeInt(playerid);
	pw.writeInt(0);
	pw.write(moving.getPacket()->getBytes(), moving.getPacket()->getLength());

	return &pw;
}
PacketWriter* PacketCreator::faceExpression(int playerid, int face){
	pw.writeShort(FACE_EEXPRESSION);

	pw.writeInt(playerid);
	pw.writeInt(face);

	return &pw;
}
PacketWriter* PacketCreator::showChatMessage(int playerid, string msg, bool macro, bool gm){
	pw.writeShort(CHAT_MESSAGE);

	pw.writeInt(playerid);
	pw.write(gm);
	pw.writeString(msg);
	pw.write(macro);

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
PacketWriter* PacketCreator::showMessage(string msg, char type, int channel, bool server){
	pw.writeShort(SHOW_MESSAGE);
	
	pw.write(type);
	if(server){
		pw.write(msg != "");
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
	pw.writeString("");
	pw.write(0);
	vector <Pet*>* pets = player->getPets();
	for(int i=0; i<(int)pets->size(); i++){
		pw.write(1);
		Pet* pet = (*pets)[i];
		pw.writeInt(pet->getItemID());
		pw.writeString(pet->getName());
		pw.write(pet->getLevel());
		pw.writeShort(pet->getCloseness());
		pw.write(pet->getFullness());
		pw.writeInt(0);
		pw.writeShort(0);
	}
	pw.write(0); // end of pets
	bool taming = player->getBuffs()->isBuffActive(Effect::MONSTER_RIDING);
	pw.write(taming); 
	if(taming){
		pw.writeInt(0); // Level
		pw.writeInt(0); // EXP
		pw.writeInt(0); // Tiredness

	}
	pw.write(0);
	pw.writeLong(1);
	pw.writeLong(0);
	pw.writeLong(0);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerReplay(string name, bool success){
	pw.writeShort(FIND_PLAYER);

	pw.write(0xA);
	pw.writeString(name);
	pw.write(success);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerMap(string name, int map){
	pw.writeShort(FIND_PLAYER);

	pw.write(0x9);
	pw.writeString(name);
	pw.write(1);
	pw.writeInt(map);
	pw.writeInt(0);
	pw.writeInt(0);

	return &pw;
}
PacketWriter* PacketCreator::findPlayerChannel(string name, int channel){
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

	IP* ip = Worlds::getInstance()->getIP();

	pw.write(ip->p1);
	pw.write(ip->p2);
	pw.write(ip->p3);
	pw.write(ip->p4);

	pw.writeShort(port);
	return &pw;
}

PacketWriter* PacketCreator::showSpecialChat(char type, string& msg, string& sender){
	pw.writeShort(SPECIAL_CHAT);

	pw.write(type);
	pw.writeString(sender);
	pw.writeString(msg);
	
	return &pw;
}