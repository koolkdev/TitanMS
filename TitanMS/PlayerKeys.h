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

#ifndef PlayerKeys_H
#define PlayerKeys_H

#include <stdlib.h>
class Key;
class Player;

class PlayerKeys {
private:
	Player* player;
	Key* keys[90];
public:
	PlayerKeys(Player* player){
		this->player = player;
		for(int i=0; i<90; i++)
			keys[i] = NULL;
	}
	Key* getKey(int n){
		if(n >= 0 && n<90)
			return keys[n];
		return NULL;
	}
	void setKey(int n, Key* key);
	void deleteAll();
};

#endif