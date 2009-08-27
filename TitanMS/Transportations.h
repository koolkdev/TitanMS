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

#ifndef TRANSPORTS_H
#define TRANSPORTS_H

#include <hash_map>
using namespace stdext;
class Trade;

class Transportations {
private:
	hash_map <int, Trade*> invites;
	//hash_map <int, Shop*> shops;
	int ids;	
public:
	Transportations(){
		ids = 1;
	}
	int getNewInviteID(Trade* trade){
		int id = ids++;
		invites[id] = trade;
		return id;
	}
	Trade* getTradeByInvite(int id){
		if(invites.find(id) == invites.end())
			return NULL;
		return invites[id];
	}
	void closeTrade(Trade* trade);

	static const int TYPE_TRADE =		3;
	static const int TYPE_SHOP =		4;

	static const int OPEN =				0;
	static const int INVITE =			2;
	static const int DECLINE =			3;
	static const int JOIN =				4;
	static const int SHOW =				5;
	static const int CHAT =				6;
	static const int CLOSE =			10;
	static const int OPEN_SHOP =		11;
	static const int TRADE_ADD_ITEM =	14;
	static const int ADD_MESOS =		15;
	static const int CONFIRM =			16;
	static const int SHOP_ADD_ITEM =	19;
	static const int BUY =				20;
	static const int REMOVE_ITEM =		24;
};

#endif