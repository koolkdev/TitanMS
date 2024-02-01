 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#include "Players.h"
#include "Player.h"
#include "PlayersPacket.h"
#include "Maps.h" 
#include "Mobs.h"
#include "Shops.h"
#include "Inventory.h"
#include "Drops.h"
#include "Levels.h"
#include "Server.h"
//
#include "SkillsPacket.h"

hash_map <int, Player*> Players::players;
hash_map <char*, Player*> Players::names;
short getShort(unsigned char* buf);
int getInt(unsigned char* buf);
void getString(unsigned char* buf, int len, char* out);

char* toLow(char* s){
	char* temps = new char[strlen(s)+1];
	for(unsigned int i=0; i<strlen(s); i++)
		if(s[i]>='A' && s[i]<='Z')
			temps[i]= s[i] - ('A'-'a');
		else
			temps[i] = s[i];
	return temps;
}

void Players::addPlayer(Player* player){
	players[player->getPlayerid()] = player;
	names[toLow(player->getName())] = player;
}

void Players::deletePlayer(Player* player){
	if(players.find(player->getPlayerid()) != players.end()){
		players.erase(player->getPlayerid());
	}
	if(names.find(toLow(player->getName())) != names.end()){
		names.erase(toLow(player->getName()));
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
		if(!player->isGM()) return;
		char* next_token;
		char command[90] = "";
		if(chatsize>2)
			strcpy_s(command, 90, strtok_s(chat+1, " ", &next_token));
		if(strcmp(command, "map") == 0){
			if(strlen(next_token) > 0){
				int mapid = atoi(strtok_s(NULL, " ",&next_token));
				if(Maps::info.find(mapid) != Maps::info.end())
					Maps::changeMap(player ,mapid, 0);
			}
		}
		else if(strcmp(command, "summon") == 0 || strcmp(command, "spawn") == 0){
			if(strlen(next_token) == 0) return;
			int mobid = atoi(strtok_s(NULL, " ",&next_token));
			if(Mobs::mobinfo.find(mobid) == Mobs::mobinfo.end())
				return;
			int count = 1;
			if(strlen(next_token) > 0)
				count = atoi(next_token);
			for(int i=0; i<count && i<100; i++){
				Mobs::spawnMob(player, mobid);
			}
		}
		else if(strcmp(command, "notice") == 0){
			if(strlen(next_token) == 0) return;
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
			if(strlen(next_token) == 0) return;
			int itemid = atoi(strtok_s(NULL, " ",&next_token));
			if(Drops::items.find(itemid) == Drops::items.end() && Drops::equips.find(itemid) == Drops::equips.end())
				return;
			int count = 1;
			if(strlen(next_token) > 0)
				count = atoi(next_token);
			Inventory::addNewItem(player, itemid, count);
		}
		else if(strcmp(command, "level") == 0){
			if(strlen(next_token) == 0) return;
			Levels::setLevel(player, atoi(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "job") == 0){
			if(strlen(next_token) == 0) return;
			Levels::setJob(player, atoi(strtok_s(NULL, " ",&next_token)));
		}	
		else if(strcmp(command, "ap") == 0){
			if(strlen(next_token) == 0) return;
			player->setAp(player->getAp()+atoi(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "sp") == 0){
			if(strlen(next_token) == 0) return;
			player->setSp(player->getSp()+atoi(strtok_s(NULL, " ",&next_token)));
		}
		else if(strcmp(command, "killnpc") == 0){
			player->setNPC(NULL);
		}
		else if(strcmp(command, "killall") == 0){
			int size=Mobs::mobs[player->getMap()].size();
			for (int j=0; j<size; j++){
				Mobs::dieMob(player, Mobs::mobs[player->getMap()][0]);
			}
		}
		else if(strcmp(command, "horntail") == 0){
			Mobs::spawnMob(player, 8810002);
			Mobs::spawnMob(player, 8810003);
			Mobs::spawnMob(player, 8810004);
			Mobs::spawnMob(player, 8810005);
			Mobs::spawnMob(player, 8810006);
			Mobs::spawnMob(player, 8810007);
            Mobs::spawnMob(player, 8810008);
            Mobs::spawnMob(player, 8810009);
        }
		else if(strcmp(command, "heal") == 0){
            player->setHP(player->getMHP());
            player->setMP(player->getMMP());
        }
		else if(strcmp(command, "kill") == 0){
			if(strcmp(next_token, "all") == 0){
				for (unsigned int x=0; x<Maps::info[player->getMap()].Players.size(); x++){
					Player* killpsa;
					killpsa = Maps::info[player->getMap()].Players[x];
					if(killpsa != player){
						killpsa->setHP(0);
					}
				}
			}
			else if(strcmp(next_token, "gm") == 0){
				for (unsigned int x=0; x<Maps::info[player->getMap()].Players.size(); x++){	
					Player* killpsa;
					killpsa = Maps::info[player->getMap()].Players[x];
					if(killpsa != player){
						if(killpsa->isGM()){	
							killpsa->setHP(0);
						}
					}
				}
			}
			else if(strcmp(next_token, "players") == 0){
				for (unsigned int x=0; x<Maps::info[player->getMap()].Players.size(); x++){	
					Player* killpsa;
					killpsa = Maps::info[player->getMap()].Players[x];
					if(killpsa != player){
						if(!killpsa->isGM()){
							killpsa->setHP(0);
						}
					}
				}
			}
			else if(strcmp(next_token, "me") == 0){
				player->setHP(0);
			}
			else {
				for (unsigned int x=0; x<Maps::info[player->getMap()].Players.size(); x++){
					Player* killpsa;
					killpsa = Maps::info[player->getMap()].Players[x];
					if(killpsa != player){
						killpsa->setHP(0);
					}
				}
			}
		}
		else if(strcmp(command, "zakum") == 0){
			Mobs::spawnMob(player, 8800000);
			Mobs::spawnMob(player, 8800003);
			Mobs::spawnMob(player, 8800004);
			Mobs::spawnMob(player, 8800005);
			Mobs::spawnMob(player, 8800006);
			Mobs::spawnMob(player, 8800007);
			Mobs::spawnMob(player, 8800008);
			Mobs::spawnMob(player, 8800009);
			Mobs::spawnMob(player, 8800010);
		}
		else if(strcmp(command, "header") == 0){
			if(strlen(next_token) > 0){
				Server::changeScrollingHeader(next_token);
			}
			else{
				Server::disableScrollingHeader();
			}
		}
		return;
	}
	PlayersPacket::showChat(player, Maps::info[player->getMap()].Players, chat);

}

void Players::damagePlayer(Player* player, unsigned char* packet){
	int damage = getInt(packet+5);
	int mobid = getInt(packet+13);
	Mob* mob = NULL;
	for(unsigned int i=0; i<Mobs::mobs[player->getMap()].size(); i++)
		if(Mobs::mobs[player->getMap()][i]->getID() == mobid){
			mob = Mobs::mobs[player->getMap()][i];
			break;
		}

	if(player->skills->getActiveSkillLevel(2001002) > 0){
		unsigned short mp = player->getMP();
        unsigned short hp = player->getHP();
        unsigned short reduc = Skills::skills[2001002][player->skills->getActiveSkillLevel(2001002)].x;
        int mpdamage = ((damage * reduc) / 100);
        int hpdamage = damage - mpdamage;
        if (mpdamage >= mp) {
            player->setMP(0);
            player->setHP(hp - (hpdamage + (mpdamage - mp)));
        }
        if (mpdamage < mp) {
            player->setMP(mp - mpdamage);
            player->setHP(hp - hpdamage);
        }
	}
	else{
		player->setHP(player->getHP()-damage);
	}
	if(mob != NULL)
		PlayersPacket::damagePlayer(player, Maps::info[player->getMap()].Players, damage, mob->getMobID());
}

void Players::healPlayer(Player* player, unsigned char* packet){
	player->setHP(player->getHP()+getShort(packet+4));
	player->setMP(player->getMP()+getShort(packet+6));
}

void Players::getPlayerInfo(Player* player, unsigned char* packet){
	PlayersPacket::showInfo(player, players[getInt(packet+4)]);
}

void Players::searchPlayer(Player* player, unsigned char* packet){
	char type = packet[0];
	int namelen = getShort(packet+1);
	if(namelen>15)
		return;
	char name[20];
	getString(packet+3, namelen, name);
	if(type == 5){ // find
		if(names.find(toLow(name)) == names.end()){
			PlayersPacket::findPlayer(player, name, -1);
		}
		else {
			PlayersPacket::findPlayer(player, names[toLow(name)]->getName(), names[toLow(name)]->getMap());
		}
	}

}

void Players::findResponse(unsigned char* packet){
	int playerid = getInt(packet);
	char channel = packet[4];
	short namelen = getShort(packet+5);
	char name[15];
	getString(packet+7, namelen, name);
	if(players.find(playerid) != players.end())
		PlayersPacket::findPlayer(players[playerid], name, -channel-2);

}
void Players::changeChannel(unsigned char* packet){
	int playerid = getInt(packet);
	char channelid = packet[4];
	Player* player = NULL;
	if(players.find(playerid) != players.end())
		player = players[playerid];
	else
		return;
	short port = getShort(packet+9);
	player->save();
	PlayersPacket::changeChannel(player, channelid, port, packet[5], packet[6], packet[7], packet[8]);
	

}