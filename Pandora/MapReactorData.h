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

#ifndef MAPREACTORDATA_H
#define MAPREACTORDATA_H

#include "DataStruct.h"

class MapReactorData : public DataStruct {
private:
	int mapid;
	int x;
	int y;
	int delay;
public:
	MapReactorData(){
		id=-1;
		mapid=0;
		x = 0;
		y = 0;
		delay = 0;
	}
	void setInMapID(int mapid){
		this->mapid = mapid;
	}
	int getInMapID(){
		return mapid;
	}
	void setX(int x){
		this->x = x;
	}
	int getX(){
		return x;
	}
	void setY(int y){
		this->y = y;
	}
	int getY(){
		return y;
	}
	void setDelay(int delay){
		this->delay = delay;
	}
	int getDelay(){
		return delay;
	}

};

#endif