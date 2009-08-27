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

#ifndef PETDATA_H
#define PETDATA_H

#include "DataStruct.h"
#include "TopDataStruct.h"
using namespace std;
class PetCommandData;

class PetData : public DataStruct, public TopDataStruct<PetCommandData> {
private:
	int hungry;
	int nameTag;
	int chatBalloon;
	string name;
	int life;
public:
	PetData(int id){
		this->id = id;
		hungry=0;
		nameTag=0;
		chatBalloon=0;
		name="";
		life=0;
	}

	void setHungry(int hungry){
		this->hungry = hungry;
	}
	void setNameTag(int nameTag){
		this->nameTag = nameTag;
	}
	void setChatBalloon(int chatBalloon){
		this->chatBalloon = chatBalloon;
	}
	void setName(string name){
		this->name = name;
	}
	void setLife(int life){
		this->life = life;
	}
	int getHungry(){
		return hungry;
	}
	int getNameTag(){
		return nameTag;
	}
	int getChatBalloon(){
		return chatBalloon;
	}
	string getName(){
		return name;
	}
	int getLife(){
		return life;
	}
};

#endif