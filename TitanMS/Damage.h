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

#ifndef DAMAGE_H
#define DAMAGE_H

class PacketReader;

#include <vector>
#include <hash_map>

using namespace std;
using namespace stdext;

class Damage {
private:
	char speed;
	char stance;
	int mobCount;
	int attackCount;
	int skill;
	int itemslot;
	hash_map <int, vector <int>> damages;
	vector<int> mobs;
	unsigned char info;
	bool mesoexplosion;
	int charge;
public:
	Damage(PacketReader& packet, bool range = false);
	int getMobsCount(){
		return mobCount;
	}
	int getAttacksCount(){
		return attackCount;
	}
	vector<int>* getMobs(){
		return &mobs;
	}
	vector<int>* getDamageForMob(int mobid){
		return &damages[mobid];
	}	
	char getStance(){
		return stance;
	}
	char getSpeed(){
		return speed;
	}
	int getCharge(){
		return charge;
	}
	int getSkill(){
		return skill;
	}
	int getItemSlot(){
		return itemslot;
	}
	char getInfoByte(){
		return info;
	}	
};

#endif