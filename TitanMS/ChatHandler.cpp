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
#include "NPC.h"
#include "World.h"
#include "DataProvider.h"
#include "PlayerNPC.h"
#include "MapNPCs.h"
#include "Party.h"
#include "Player.h"
#include "Channel.h"
#include "Maps.h"
#include "Skill.h"
#include "PlayerSkills.h"
#include "Map.h"
#include "Event.h"
#include "MapMobs.h"
#include "Pet.h"
#include "PlayerInventories.h"
#include "Equip.h"
#include "MapDrops.h"
#include "PlayerInventories.h"
#include "AngelScriptEngine.h"
int strval(string& str);
string valstr(int str);

void PlayerHandler::chatCommandHandle(PacketReader& packet){
	unsigned char type = packet.read();

	if (type == 0x05) { // find
		string name = packet.readString();

		Player *locatePlayer = player->getChannel()->getWorld()->getPlayerByName(name);

		if (locatePlayer != NULL) {
			if(locatePlayer->getChannel() == player->getChannel()) {
				player->send(PacketCreator().findPlayerMap(name, locatePlayer->getMap()->getID()));
			}
			else {
				player->send(PacketCreator().findPlayerChannel(name, locatePlayer->getChannel()->getID()));
			}
		}
		else {
			player->send(PacketCreator().findPlayerReplay(name, 0));
		}
	}
	else if (type == 0x06) { // wishper
		string name = packet.readString();
		string text = packet.readString();

		Player *receiver = player->getChannel()->getWorld()->getPlayerByName(name);

		if (receiver != NULL) {
			receiver->send(PacketCreator().whisperPlayer(player->getName(), text, player->getChannel()->getID()));
			player->send(PacketCreator().findPlayerReplay(name, 1));         
		}
		else {
			player->send(PacketCreator().findPlayerReplay(name, 0));
		}
	}
}

void PlayerHandler::useChatHandle(PacketReader& packet){
	string msg = packet.readString();
	if(msg.substr(0, 1) == "!" && player->isGM()){
		string command = msg.substr(1, msg.find(" ")-1);
		if(command == "map"){
			int mapid = strval(msg.substr(msg.find(" ")));
			Map* map = player->getChannel()->getMaps()->getMap(mapid);
			if(map != NULL){
				if(map->getEvent() != NULL){
					player->send(PacketCreator().showMessage("You can't teleport to a map with an event.", 5));
				}
				else{
					player->changeMap(map);
				}
			}
		}
		else if(command == "shop"){
			ShopData* shop = DataProvider::getInstance()->getShopData(9999999);
			if(shop != NULL){
				player->setNPCShop(shop);	
				player->send(PacketCreator().showNPCShop(shop));	
				return;	
			}
		}
		else if(command == "item"){
			int p = msg.find(" ");
			int ap = msg.substr(p+1).find(" ");
			int itemid = 0;
			int amount = 1;
			if(ap == -1)
				itemid = strval(msg.substr(p));
			else{
				itemid = strval(msg.substr(p+1, ap));
				amount = strval(msg.substr(ap+p+2));
			}
			player->getInventories()->giveItem(itemid, amount); 
		}
		else if(command == "mesos"){
			int mesos = strval(msg.substr(msg.find(" ")));
			player->addMesos(mesos);
		}
		else if(command == "level"){
			int level = strval(msg.substr(msg.find(" ")));
			player->setLevel(level);
			player->setExp(0);
		}
		else if(command == "job"){
			int job = strval(msg.substr(msg.find(" ")));
			player->setJob(job);
		}
		else if(command == "sp"){
			int sp = strval(msg.substr(msg.find(" ")));
			player->addSP(sp);
		}
		else if(command == "ap"){
			int ap = strval(msg.substr(msg.find(" ")));
			player->addAP(ap);
		}
		else if(command == "str"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addSTR(s);
		}
		else if(command == "dex"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addDEX(s);
		}
		else if(command == "int"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addINT(s);
		}
		else if(command == "luk"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addLUK(s);
		}
		else if(command == "hp"){
			int hp = strval(msg.substr(msg.find(" ")));
			player->setHP(hp);
		}
		else if(command == "mp"){
			int mp = strval(msg.substr(msg.find(" ")));
			player->setMP(mp);
		}
		else if(command == "heal"){
			player->setHP(player->getMaxHP());
			player->setMP(player->getMaxMP());
		}
		else if(command == "maxhp"){
			int maxhp = strval(msg.substr(msg.find(" ")));
			player->setBaseMaxHP(maxhp);
			player->setMaxHP(maxhp);
		}
		else if(command == "maxmp"){
			int maxmp = strval(msg.substr(msg.find(" ")));
			player->setBaseMaxMP(maxmp);
			player->setMaxMP(maxmp);
		}
		else if(command == "mount"){
			Skill* skill = player->getSkills()->getSkill(1004);
			skill->setMasterLevel(1);
			skill->setLevel(1);
			player->send(PacketCreator().updateSkill(skill));
		}
		else if(command == "npc"){
			// todo npcs data, till this it will dc if the id is wrong
			int npcid = strval(msg.substr(msg.find(" ")));
			Position pos = player->getMap()->findFoothold(player->getPosition());
			player->getMap()->getNPCs()->spawn(new NPC(player->getMap()->getNPCs()->getNewID(), pos.x, pos.y, npcid, player->getFoothold(), pos.x-50, pos.x+50, player->getStance()%2 == 0));
		}
		else if(command == "summon" || command == "spawn"){
			int p = msg.find(" ");
			int ap = msg.substr(p+1).find(" ");
			int mob = 0;
			int amount = 1;
			if(ap == -1)
				mob = strval(msg.substr(p));
			else{
				mob = strval(msg.substr(p+1, ap));
				amount = strval(msg.substr(ap+p+2));
				amount = ( 100 < amount) ? 100 : amount;
			}
			for(int i=0; i<amount; i++)
				player->getMap()->getMobs()->spawnMob(mob, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
		}
		else if(command == "test"){
		}
		else if(command == "killall"){
			player->getMap()->clearMobs();
		}
		else if(command == "pos"){
			player->send(PacketCreator().showMessage("x: " + valstr(player->getPosition().x) + " y: " + valstr(player->getPosition().y)  + " fh: " + valstr(player->getFoothold()), 5));
		}
		else if(command == "pvp"){
			if ( player->getMap()->getPvP() == NULL ) {
				player->getMap()->startPvP();
			} else {
				player->getMap()->stopPvP();
			}
		}
		else if(command == "killnpc"){
			player->setNPC(NULL);
		}
		else if(command == "horntail"){
				player->getMap()->getMobs()->spawnMob(8810002, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810003, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810004, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810005, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810006, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810007, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
				player->getMap()->getMobs()->spawnMob(8810008, player->getPosition().x, player->getPosition().y, -1, -1, -1, player->getFoothold());
			
		}
		else if(command == "drop"){
			int p = msg.find(" ");
			int ap = msg.substr(p+1).find(" ");
			int itemid = 0;
			int amount = 1;
			if(ap == -1)
	            itemid = strval(msg.substr(p));
			else{
	            itemid = strval(msg.substr(p+1, ap));
				amount = strval(msg.substr(ap+p+2));
			}
			if(player->getInventories()->validID(itemid)){
				Item* item = NULL;
			 	if(IS_EQUIP(itemid))
		            item = new Equip(itemid, true);
				else if(IS_PET(itemid))
		            item = new Pet(itemid);
				else
		            item = new Item (itemid,amount);
				player->getMap()->getDrops()->dropFromObject(item, player->getPosition(), player);
			}
		}
		else if(command == "runevent"){
			int p = msg.find(" ");
			string runevent = msg.substr(p+1);
			AngelScriptEngine::handleEvent(player->getChannel(), runevent);
		}
		else if(command == "shiptest"){
		}
	}
	else{
		player->getMap()->send(PacketCreator().showChatMessage(player->getID(), msg, packet.read() == 1, player->isGM()));
	}

}

void PlayerHandler::specialChatHandle(PacketReader& packet){
	// TODO: check that player have guild/party/buddy and the recivers are in his party/guild/buddy, and check that player is online before sending?
	Players players;
	char type = packet.read();
	char count = packet.read();
	for(int i=0; i<count; i++){
		Player* recv = player->getChannel()->getWorld()->getPlayerByID(packet.readInt());
		if(recv != NULL)
			players.add(recv);
	}
	players.send(PacketCreator().showSpecialChat(type, packet.readString(), player->getName()));
}