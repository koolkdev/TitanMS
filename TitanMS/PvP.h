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

#ifndef PVP_H
#define PVP_H

class Map;
class Player;
class ObjectMoving;
struct Position;

class PvP {
private:
	Map* map;
public:
	PvP(Map* map);
	void startPvP();
	void endPvP();
	void hitPlayer(Player* hitter, Player* player, int damage);
	void movePlayer(Player* player, Position& pos, ObjectMoving& moving);
	void showPvP(Player* player);
};

#endif