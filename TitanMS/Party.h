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

#ifndef PARTY_H
#define PARTY_H

#include <hash_map>
#include <string>
class PartyMembers;
class Player;
class World;
class Map;

class Party{
private:
	int id;
	PartyMembers* members;
	World* world;
	int leader;
	stdext::hash_map <std::string, int> vars;
public:
	Party(World* world, Player* player);
	Party(int id, int leader);
	~Party();
	int getID(){
		return id;
	}
	PartyMembers* getMembers(){
		return members;
	}
	int getLeader(){
		return leader;
	}
	void setLeader(int leader){
		this->leader = leader;
	}
	bool checkMembers(int members, Map* map, int minlevel, int maxlevel);
	void removeMember(int playerid);
	void setVariable(std::string &name, int val){
		vars[name] = val;
	}
	int getVariable(std::string &name){
		if(vars.find(name) == vars.end())
			return -1;
		else
			return vars[name];
	}
	void deleteVariable(std::string &name){
		if(vars.find(name) != vars.end())
			vars.erase(name);
	}
	void clear(){
		vars.clear();
	}
};


#endif