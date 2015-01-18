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

#include "Party.h"
#include "Player.h"
#include "PartyMembers.h"
#include "Map.h"
#include "World.h"
#include <Winsock2.h>
#include "MySQLM.h"
#include "..\mysql++\mysql++.h"

string valstr(int id);

Party::Party(World* world, Player* player){
	members = new PartyMembers();
	id = (int)MySQL::getInstance()->getDataBase()->query("insert into party(world, leader) values(" + valstr(world->getID()) + "," + valstr(player->getID()) + ")").execute().insert_id();
	MySQL::getInstance()->getDataBase()->query("update characters set party=" + valstr(id) + " where id=" + valstr(player->getID())).execute();
	members->addMember(player);
	leader = player->getID();
}
Party::Party(int id, int leader){
	members = new PartyMembers();
	this->id = id;
	this->leader = leader;
}
Party::~Party(){
	MySQL::getInstance()->getDataBase()->query("delete from party where id=" + valstr(id)).execute();
	struct RemoveParty {
		void operator()(Player* player){
			player->setParty(NULL);
		}
	} removeParty;
	members->run(&removeParty);
	//		MySQL::getInstance()->getDataBase()->query("update characters set party=0 where party=" + valstr(id)).execute();
	delete members;
}

void Party::removeMember(int playerid){
	members->deleteMember(playerid);
	//MySQL::getInstance()->getDataBase()->query("update characters set party=0 where id=" + valstr(playerid)).execute();
}