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

#ifndef PLAYERBUFFS_H
#define PLAYERBUFFS_H

#include <vector>
#include <hash_map>

class Buff;
class Player;

using namespace std;
using namespace stdext;

class PlayerBuffs {
private:
	Player* player;
	hash_map <__int64, Buff*> buffs;
	hash_map <int, Buff*> idbuffs;
public:
	PlayerBuffs(Player* player){
		this->player = player;
	}
	bool isBuffActive(__int64 buff){
		return (buffs.find(buff) != buffs.end());
	}
	int getBuffValue(__int64 buff);
	void cancelBuff(int buffid, bool timer = false);
	void addBuff(Buff* buff);
	void deleteBuff(Buff* buff, bool timer = false);
	void cancelAll();
};

#endif