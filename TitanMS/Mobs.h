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

#ifndef MOBS_H
#define MOBS_H

#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

#include "Player.h"

class Player;
class Mob;

struct SpawnInfo {
	int id;
	short x;
	short cy;
	short fh;
	int rate;
	int last;
};
typedef vector<SpawnInfo> SpawnsInfo;

struct MobInfo {
	int hp;
	int mp;
	int exp;
	vector<int> summon;
};



class Mobs {
public:
	static hash_map <int, MobInfo> mobinfo;
	static hash_map <int, SpawnsInfo> info;
	static hash_map <int, vector<Mob*>> mobs;
	static int mobscount;
	static void addMob(int id, MobInfo mob){
		mobinfo[id] = mob;
	}
	static void addSpawn(int id, SpawnsInfo spawns){
		info[id] = spawns;
	}
	static void damageMob(Player* player, unsigned char* packet);
	static void damageMobS(Player* player, unsigned char* packet, int size);
	static void damageMobSkill(Player* player, unsigned char* packet);
	static void monsterControl(Player* player, unsigned char* packet, int size);
	static void monsterControlSkill(Player* player, unsigned char* packet){}
	static void checkSpawn(int mapid);
	static void showMobs(Player* player);
	static void updateSpawn(int mapid);
	static void spawnMob(Player* player, int mobid);
	static void spawnMob(int mapid, int mobid);
	static void dieMob(Player* player, Mob* mob);
	static void spawnMobPos(Player* player, int mobid, int xx, int yy);
	static Player* Mobs::getClosetPlayer(int mapid, Mob* mob);
	static Mob* getMobByID(int mobid, int map);
};


#endif