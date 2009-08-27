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

#ifndef QUESTREQUESTDATA_H
#define QUESTREQUESTDATA_H

#include "DataStruct.h"

class QuestRequestData : public DataStruct {
private:
	bool item;
	bool mob;
	bool quest;
	int count;
public:
	QuestRequestData(){
		id=-1;
		item = 0;
		mob = 0;
		quest = 0;
		count = 0;
	}

	void setItem(bool item){
		this->item = item;
	}
	bool getItem(){
		return item;
	}
	void setMob(bool mob){
		this->mob = mob;
	}
	bool getMob(){
		return mob;
	}
	void setQuest(bool quest){
		this->quest = quest;
	}
	bool getQuest(){
		return quest;
	}
	void setCount(int count){
		this->count = count;
	}
	int getCount(){
		return count;
	}

};

#endif