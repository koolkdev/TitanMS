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

#ifndef PARTYMEMBERS_H
#define PARTYMEMBERS_H

class PartyMembers;
class PartyMember;
#include "Players.h"

class PartyMembers : public Players {
private:
	hash_map <int, PartyMember*> members;
public:
	~PartyMembers();

	hash_map <int, PartyMember*>* getMembers(){
		return &members;
	}
	PartyMember* getMember(int id){
		if(members.find(id) == members.end())
			return NULL;
		return members[id];
	}
	void addMember(Player* player);
	void addMember(PartyMember* member);
	void deleteMember(int id);
	void removeMember(int id);
};

#endif