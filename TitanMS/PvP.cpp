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
#include "PvP.h"
#include "Map.h"
#include "Player.h"
#include "MapPlayers.h"
#include "PacketCreator.h"

//#define MASKED_MOB 9400121
#define MASKED_MOB 9409000
//#define MASKED_MOB 100100

PvP::PvP(Map* map){
	this->map = map;
}
void PvP::startPvP(){
	vector <Player*>* players = map->getPlayers()->getPlayersList();
	for(int i=0; i<(int)players->size(); i++){
		(*players)[i]->setPvP(true);
		(*players)[i]->setPvPMaskMob(MASKED_MOB); // temp, by level
		map->send(PacketCreator().startPvPMob((*players)[i]), (*players)[i]);
		(*players)[i]->send(PacketCreator().startPvPMobForPlayer((*players)[i]));
		//map->send(PacketCreator().startPvPMobEffect((*players)[i], (*players)[i]->getWDef(), (*players)[i]->getMDef()));
		//map->send(PacketCreator().updatePvPHP((*players)[i]));
	}
}
void PvP::endPvP(){
	vector <Player*>* players = map->getPlayers()->getPlayersList();
	for(int i=0; i<(int)players->size(); i++){
		(*players)[i]->setPvP(false);
		(*players)[i]->send(PacketCreator().stopPvPForPlayer((*players)[i]));
		map->send(PacketCreator().stopPvP((*players)[i]), (*players)[i]);
	}

}
void PvP::hitPlayer(Player* hitter, Player* player, int damage){
	player->addHP(-damage);
	map->send(PacketCreator().updatePvPHP(player));
	if(player->getHP() <= 0){
		player->send(PacketCreator().stopPvPForPlayer(player));
		map->send(PacketCreator().stopPvP(player), (player));
	}
}
void PvP::movePlayer(Player* player, Position& pos, ObjectMoving& moving){
	map->send(PacketCreator().showPvPMoving(player, pos, moving));
	map->send(PacketCreator().showPvPMovingForPlayer(player, pos));
}
void PvP::showPvP(Player* player){
	player->setPvP(true);
	player->setPvPMaskMob(MASKED_MOB);
	vector <Player*>* players = map->getPlayers()->getPlayersList();
	for(int i=0; i<(int)players->size(); i++){
		if((*players)[i]!=player){
			player->send(PacketCreator().startPvPMob((*players)[i]));
			//player->send(PacketCreator().startPvPMobEffect((*players)[i], (*players)[i]->getWDef(), (*players)[i]->getMDef()));
			//player->send(PacketCreator().updatePvPHP((*players)[i]));
		}
	}
	map->send(PacketCreator().startPvPMob(player), player);
	player->send(PacketCreator().startPvPMobForPlayer(player));
	//map->send(PacketCreator().startPvPMobEffect(player, player->getWDef(), player->getMDef()));
	//map->send(PacketCreator().updatePvPHP(player));
}