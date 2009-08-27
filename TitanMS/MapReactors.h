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

#ifndef MAPREACTOR_H
#define MAPREACTOR_H


#include <vector>

using namespace std;

class Mob;
class Reactor;
class Player;
class Drop;

class MapReactors {
private:
	static int ids;
	Map* map;
	vector <Reactor*> reactors;
	stdext::hash_map <int, int> last;
	bool timeron;
public:
	MapReactors(Map* map);
	Reactor* getReactorByID(int objid);
	void show(Player* player);
	void checkSpawn(bool timer = false);
	void show(Reactor* reactor);
	void hitReactor(Reactor* reactor);
	void hitReactor(Reactor* reactor, Player* player);
	void hitReactor(Reactor* reactor, Player* player, int stance, int pos);
	void destroy(Reactor* reactor);
	void checkDrop(Drop* drop, Player* player);
	Reactor* spawnReactor(int reactorid, int x, int y, int orgID = -1);
	Reactor* getReactorByReactorID(int id);
	void clear();

};

#endif