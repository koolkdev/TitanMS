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

#ifndef DROPDATA_H
#define DROPDATA_H

#include "DataStruct.h"

class DropData : public DataStruct {
private:
	int chance;
	int quest;
public:
	DropData(){
		id=-1;
		chance=0;
		quest=0;
	}

	void setChance(int chance){
		this->chance = chance;
	}
	void setQuest(int quest){
		this->quest = quest;
	}
	int getChance(){
		return chance;
	}
	int getQuest(){
		return quest;
	}

};

#endif