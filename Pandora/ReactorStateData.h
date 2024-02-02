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

#ifndef REACTORSTATEDATA_H
#define REACTORSTATEDATA_H

#include "Tools.h"
#include "DataStruct.h"

class ReactorStateData : public DataStruct {
private:
	int type;
	int nextState;
	int itemid, amount;
	Position rb,lt;
public:
	ReactorStateData(){
		id = -1;
		type = 0;
		nextState = 0;
		itemid =0;
		amount =0;
	}
	void setType(int t){
		type = t;
	}
	void setNextState(int nt){
		nextState = nt;
	}
	void setRB(Position r){
		rb = r;
	}
	void setLT(Position l){
		lt = l;
	}
	void setRBX(int x){
		rb.x = x;
	}
	void setRBY(int y){
		rb.y = y;
	}
	void setLTX(int x){
		lt.x = x;
	}
	void setLTY(int y){
		lt.y = y;
	}
	int getType(){
		return type;
	}
	int getNextState(){
		return nextState;
	}
	Position& getRB(){
		return rb;
	}
	Position& getLT(){
		return lt;
	}
	void setItemID(int id){
		itemid = id;
	}
	int getItemID(){
		return itemid;
	}
	void setAmount(int a){
		amount = a;
	}	
	int getAmount(){
		return amount;
	}

};

#endif