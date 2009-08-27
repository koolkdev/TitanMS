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

#ifndef MAPMOBS_H
#define MAPMOBS_H

#include <vector>

using namespace std;

class Mob;
class Map;
class Player;
class Damage;
class Item;

class MapMobs {
private:
	static int ids;
	Map* map;
	vector <Mob*> mobs;
	stdext::hash_map <int, int> last;
	bool timeron;
public:
	MapMobs(Map* map);
	Mob* getMobByID(int objid);
	void show(Player* player);
	void checkSpawn(bool timer = false);
	void show(Mob* mob, bool spawn = true);
	void damage(Player* player, Mob* mob, int damage);
	void damage(Player* player, Damage& dmg, Item* item = NULL);
	void kill(Mob* mob, Player* by);
	void summonFromMob(Mob* from);
	Mob* spawnMob(int mobid, int x, int y, int orgID = -1, char type = -1, char effect = -1, int fh = 0, bool flip = false, bool find = true, int from = 0);
	void clear();
	int getMobsCount();

};

#endif