#include "Players.h"
#include "Player.h"
#include "PlayersPacket.h"
#include "Maps.h"
#include "Mobs.h"
#include "Shops.h"
#include "Inventory.h"
#include "Drops.h"
#include "Levels.h"

hash_map <int, Player*> Players::players;
short getShort(unsigned char* buf);
int getInt(unsigned char* buf);
int strval(char* buf);
void getString(unsigned char* buf, int len, char* out);

void Players::addPlayer(Player* player){
	players[player->getPlayerid()] = player;
}

void Players::deletePlayer(Player* player){
	if(players.find(player->getPlayerid()) != players.end()){
		for (hash_map<int,Player*>::iterator iter = players.begin();
			 iter != players.end(); iter++){
				 if(iter->first == player->getPlayerid()){
					 players.erase(iter);
					break;
				 }
		}
	}
}

void Players::handleMoving(Player* player, unsigned char* packet, int size){
	Pos cpos;
	int pla = packet[5]*14+1;
	cpos.x = getShort(packet+size-4);
	cpos.y = getShort(packet+size-2);
	player->setPos(cpos);
	player->setType(packet[5+14*(packet[5]-1)+12]);
	PlayersPacket::showMoving(player, Maps::info[player->getMap()].Players, packet, pla);
}

void Players::faceExperiment(Player* player, unsigned char* packet){
	int face = getInt(packet);
	PlayersPacket::faceExperiment(player, Maps::info[player->getMap()].Players, face);
}
void Players::chatHandler(Player* player, unsigned char* packet){
	char chat[91];
	int chatsize = getShort(packet);
	getString(packet+2, chatsize, chat);
	if(chat[0] == '!'){
		char* next_token;
		char command[90] = "";
		if(chatsize>2)
			strcpy_s(command, 90, strtok_s(chat+1, " ", &next_token));
		if(strcmp(command, "map") == 0){
			if(strlen(next_token) > 0){
				int mapid = strval(strtok_s(NULL, " ",&next_token));
				if(Maps::info.find(mapid) != Maps::info.end())
					Maps::changeMap(player ,mapid, 0);
			}
		}
		else if(strcmp(command, "summon") == 0 || strcmp(command, "spawn") == 0){
			int mobid = strval(strtok_s(NULL, " ",&next_token));
			if(Mobs::mobinfo.find(mobid) == Mobs::mobinfo.end())
				return;
			int count = 1;
			if(strlen(next_token) > 0)
				count = strval(next_token);
			for(int i=0; i<count && i<100; i++){
				Mobs::spawnMob(player, mobid);

			}
		}
		else if(strcmp(command, "notice") == 0){
			PlayersPacket::showMassage(next_token, 0);
		}
		else if(strcmp(command, "shop") == 0){
			Shops::showShop(player, 9999999);
		}
		else if(strcmp(command, "shop2") == 0){
			Shops::showShop(player, 9999998);
		}
		else if(strcmp(command, "pos") == 0){
			char text[50];
			sprintf_s(text, 50, "X: %d Y: %d", player->getPos().x, player->getPos().y);
			PlayersPacket::showMassage(text, 0);
		}
		else if(strcmp(command, "item") == 0){
			int itemid = strval(strtok_s(NULL, " ",&next_token));
			if(Drops::items.find(itemid) == Drops::items.end() && Drops::equips.find(itemid) == Drops::equips.end())
				return;
			int count = 1;
			if(strlen(next_token) > 0)
				count = strval(next_token);
			 Inventory::addNewItem(player, itemid, count);
		}
		else if(strcmp(command, "level") == 0){
			Levels::setLevel(player, strval(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "job") == 0){
			Levels::setJob(player, strval(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "ap") == 0){
			player->setAp(player->getAp()+strval(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "sp") == 0){
			player->setSp(player->getSp()+strval(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "killnpc") == 0){
			player->setNPC(NULL);
		}
		return;
	}
	PlayersPacket::showChat(player, Maps::info[player->getMap()].Players, chat);

}

void Players::damagePlayer(Player* player, unsigned char* packet){
	int damage = getInt(packet+5);
	int mobid = getInt(packet+13);
	player->setHP(player->getHP()-damage);
	PlayersPacket::damagePlayer(player, Maps::info[player->getMap()].Players, damage, mobid);
}

void Players::healPlayer(Player* player, unsigned char* packet){
	player->setHP(player->getHP()+getShort(packet+4));
	player->setMP(player->getMP()+getShort(packet+6));
}