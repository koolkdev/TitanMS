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
#include "World.h"
#include "Channel.h"
#include "Channels.h"
#include "Players.h"
#include "Parties.h"

World::World(int id, int channels){
	this->id = id;
	this->channels = new Channels(this, channels);
	parties = new Parties(this);
}

bool World::isPlayerConnected(int charid){
	for(int i=0; i<channels->getChannelsCount(); i++){
		if(channels->getChannel(i)->getPlayers()->getPlayerByID(charid) != NULL)
			return true;
	}
	return false;
}

// TODO: add players object?
Player* World::getPlayerByName(string& name){
	for(int i=0; i<channels->getChannelsCount(); i++){
		Player* player = channels->getChannel(i)->getPlayers()->getPlayerByName(name);
		if(player != NULL){
			return player;
		}
	}
	return NULL;
}

Player* World::getPlayerByID(int id){
	for(int i=0; i<channels->getChannelsCount(); i++){
		Player* player = channels->getChannel(i)->getPlayers()->getPlayerByID(id);
		if(player != NULL){
			return player;
		}
	}
	return NULL;
}