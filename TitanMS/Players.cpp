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

#include "Players.h"
#include "Player.h"
#include "PacketWriter.h"
#include "Tools.h"
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace Tools;

Player* Players::getPlayerByName(string tname){
	string name = string(tname);
	transform(name.begin(),name.end(),name.begin(),tolower);
	if(names.find(name) != names.end())
		return names[name];
	return NULL;	
}
Player* Players::getPlayerByID(int id){
	if(players.find(id) != players.end())
		return players[id];
	return NULL;	
}

void Players::add(Player* player){
	players[player->getID()] = player;
	string name = string(player->getName());
	transform(name.begin(),name.end(),name.begin(),tolower);
	names[name] = player;
}

void Players::remove(Player* player){
	if(players.find(player->getID()) != players.end()){
		players.erase(player->getID());
	}
	string name = string(player->getName());
	transform(name.begin(),name.end(),name.begin(),tolower);
	if(names.find(name) != names.end()){
		names.erase(name);
	}
}
void Players::send(PacketWriter* pw){
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		iter->second->send(pw);
	}
}
void Players::send(PacketWriter* pw, Player* player){
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		if(iter->second != player)
			iter->second->send(pw);
	}
}
void Players::send(PacketWriter* pw, Player* player, Position pos){
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		if(iter->second != player && getDistance(iter->second->getPosition(), pos) <= 1000*1000)
			iter->second->send(pw);
	}
}
void Players::send(PacketWriter* pw, Map* map){
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		if(iter->second->getMap() == map)
			iter->second->send(pw);
	}
}
void Players::send(PacketWriter* pw, Map* map, Player* player){
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		if(iter->second->getMap() == map && iter->second != player)
			iter->second->send(pw);
	}
}

void Players::changeMap(int mapid){
	struct ChangeMap {
		void operator()(Player* player){
			player->setMap(map);
		}
		int map;
	} changeMap = {mapid};
	run(&changeMap);
}
void Players::changeMapPos(int mapid, int pos){
	struct ChangeMap { 
		void operator()(Player* player){
			player->setMapPos(map, pos);
		}
		int map;
		int pos;
	} changeMap = {mapid, pos};
	run(&changeMap);
}
void Players::changeMapPortal(int mapid, string& portalname){
	struct ChangeMap  { 
		void operator()(Player* player){
			player->setMapPortal(map, portal);
		}
		int map;
		string portal;
	} changeMap = {mapid, portalname};
	run(&changeMap);
}/*
void Players::run(RunAction* act){
	hash_map<int,Player*> players = hash_map<int, Player*>(this->players);
	for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
		(*act)(iter->second);
	}
}*/

Player* Players::operator[](int index){
	if(players.size() == 0) return NULL;
	hash_map<int, Player*>::iterator iter = players.begin();
	for(int i=0; i<index; i++){
		iter++;
		if(iter == players.end()) return players.begin()->second;		
	}
	return iter->second;
}