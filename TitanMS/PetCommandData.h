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

#ifndef PETCHATCOMMANDDATA_H
#define PETCHATCOMMANDDATA_H

#include "DataStruct.h"

class PetCommandData : public DataStruct {
private:
	int closeness;
	int prob;
	int minLevel;
	int maxLevel;
public:
	PetCommandData(int id){
		this->id = id;
		closeness = 0;
		prob = 0;
		minLevel = 0;
		maxLevel = 0;
	}
	void setCloseness(int c){
		closeness = c;
	}
	void setProb(int p){
		prob = p;
	}
	void setMinLevel(int l){
		minLevel = l;
	}
	void setMaxLevel(int l){
		maxLevel = l;
	}
	int getCloseness(){
		return closeness;
	}
	int getProb(){
		return prob;
	}
	int getMinLevel(){
		return minLevel;
	}
	int getMaxLevel(){
		return maxLevel;
	}
	

};

#endif