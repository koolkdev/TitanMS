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
#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <hash_map>
#include <string>
#include "Players.h"
class Buddy;
class BuddyList;

class BuddyList : public Players {
private:
	Player* player;
	stdext::hash_map <int, Buddy*> buddies;
public:
	BuddyList(Player* player){
		this->player = player;
	}
	~BuddyList();

	stdext::hash_map <int, Buddy*>* getBuddies(){
		return &buddies;
	}
	Buddy* getBuddy(int id){
		if(buddies.find(id) == buddies.end())
			return NULL;
		return buddies[id];
	}
	void invite(std::string name);
	void accept(std::string name);
	void remove(std::string name);
};

#endif