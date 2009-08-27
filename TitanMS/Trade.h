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

#ifndef TRADE_H
#define TRADE_H

#include <hash_map>
#include <string>
using namespace stdext;
using namespace std;
class Player;
class Item;

class Trade {
private:
	Player* player;
	Trade* trader;
	int inv;
	Player* invited;
	vector<pair<int,Item*>> items;
	int mesos;
	bool locked;
public:
	Trade(Player* player);
	void open();
	bool invite(Player* to);
	void close();
	void decline();
	void accept();
	void chat(string msg);
	void addMesos(int mesos);
	void addItem(Item* item, short slot);
	void complete();
	void confirm();
	int getInviteID(){
		return inv;
	}
	Player* getPlayer(){
		return player;
	}
	Player* getInvited(){
		return invited;
	}
	Trade* getTrader(){
		return trader;
	}
	void setTrader(Trade* t){
		trader= t;
	}
	bool isConfirmed(){
		return locked;
	}
	int getMesos(){
		return mesos;
	}
	vector <pair<int, Item*>>* getItems(){
		return &items;
	}
};

#endif