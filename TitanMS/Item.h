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

#ifndef ITEM_H
#define ITEM_H

#include <string>

using namespace std;

class Item {
protected:
	int id;
	int amount;
	int slot;
	int type;
	bool lock;
public:
	Item();
	Item(int itemid, int amount=1);
	Item(const Item &item): id(item.id), amount(item.amount), slot(item.slot), type(item.type), lock(item.lock){}
	void setID(int id);
	int getID(){
		return id;
	}
	void setAmount(int amount){
		this->amount = amount;
	}
	int getAmount(){
		return amount;
	}
	void setSlot(int slot){
		this->slot = slot;
	}
	int getSlot(){
		return slot;
	}
	int getType(){
		return type;
	}
	void setLocked(bool b){
		lock = b;
	}
	bool getLocked(){
		return lock;
	}
	bool isBullet();
};

#endif