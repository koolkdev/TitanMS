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

#ifndef DROPSDATA_H
#define DROPSDATA_H


#include "TopDataStruct.h"

class DropData;

class DropsData: public TopDataStruct <DropData>, public DataStruct {
private:
	int dropslimit;
	int maxmesos;
	int minmesos;
	int maxbags;
	int minbags;
public:
	DropsData(int id){
		this->id = id;
		dropslimit=-1;
		maxmesos=0;
		minmesos=0;
		maxbags=1;
		minbags=1;
	}
	void setDropsLimit(int dl){
		dropslimit = dl;
	}
	int getDropsLimit(){
		return dropslimit;
	}
	void setMaxMesos(int max){
		maxmesos = max;
	}
	void setMinMesos(int min){
		minmesos = min;
	}
	int getMaxMesos(){
		return maxmesos;
	}
	int getMinMesos(){
		return minmesos;
	}
	void setMaxBags(int max){
		maxbags = max;
	}
	void setMinBags(int min){
		minbags = min;
	}
	int getMaxBags(){
		return maxbags;
	}
	int getMinBags(){
		return minbags;
	}
	 
};

#endif