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
#include <vector>

using namespace std;
using namespace stdext;

class Player;
class Party;

class PartyMember {
private:
	Player* player;
	int playerid;
	bool leader;
	int status;
public:
	PartyMember(int playerid, Party* party, bool lead = false);
};
class Party {
private:
	int id;
	vector <PartyMember*> members;
	static hash_map <int, Party*> parties;
public:
	Party(Player* player);
	int getPartyID(){
		return id;
	}
};

#endif