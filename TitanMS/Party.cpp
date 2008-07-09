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
/*
#include "Party.h"
#include "Player.h"
#include "Players.h"
#include "PartyPacket.h"

hash_map <int, Party*> Party::parties;

PartyMember::PartyMember(int playerid, Party* party, bool lead){
	this->player = Players::getPlayerByID(playerid);
	leader = lead;
	if(player == NULL){
		status = 0;
		Players::getPlayerStatus(playerid);
	}
	else
		status = 1;
	player->setParty(party);
	player->setPartyMember(this);
}	

Party::Party(Player* player){
	members.push_back(new PartyMember(player->getPlayerid(), this, true));
	parties[player->getPlayerid()] = this;
	PartyPacket::createParty(player);
}*/