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
#include "MapData.h"
#include "Player.h"
#include "Players.h"
#include "MapPlayers.h"
#include "Event.h"
#include "MapsData.h"
#include "PacketCreator.h"
#include "Map.h"
#include "Tools.h"

void MapPlayers::add(Player* player){
	players[player->getID()] = player;
	playersv.push_back(player);
	map->send(PacketCreator().showPlayer(player));
	for(int i=0; i<(int)playersv.size(); i++){
		if(playersv[i] != player)
			player->send(PacketCreator().showPlayer(playersv[i]));
	}
	if(map->getEvent() != NULL){
		send(PacketCreator().showTimer(map->getEvent()->getTime()));
	}
	if(map->getMusic() != MapsData::getInstance()->getDataByID(map->getID())->getMusic()){
		player->send(PacketCreator().changeMusic(map->getMusic()));
	}
	if(MapsData::getInstance()->getDataByID(map->getID())->getClock()){
		player->send(PacketCreator().showClock(Tools::getTime()));
	}
	if(map->getType() == 2){
		if(MapsData::getInstance()->getDataByID(map->getID())->getShip()->getType() == 0)
			player->send(PacketCreator().showShip(map->getShip() ? 1 : 3, 0));
		else if(map->getShip()){
			player->send(PacketCreator().showShip(4, 1));
		}
	}
	else if(map->getType() == 82){
		player->send(PacketCreator().makeApple());
	}
}
void MapPlayers::remove(Player* player){
	if(players.find(player->getID()) != players.end()){
		players.erase(player->getID());
	}
	for(int i=0; i<(int)playersv.size(); i++){
		if(playersv[i] == player){
			playersv.erase(playersv.begin()+i);
			break;
		}
	}
	map->send(PacketCreator().removePlayer(player->getID()), player);
}