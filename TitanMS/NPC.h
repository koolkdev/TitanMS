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

#ifndef NPCS_H
#define NPCS_H

#include "LifeMapObject.h"

class MapNPCData;

class NPC : public LifeMapObject {
private:
	int npcid;
	int rx0;
	int rx1;
	bool flip;
public:
	NPC(int id, MapNPCData* data);
	NPC(int id, int x, int y, int npcid, int fh =0, int rx0 = 0, int rx1 =0, bool flip=0){
		this->id = id;
		Position pos;
		pos.x = x;
		pos.y = y;
		this->pos = pos;
		this->npcid = npcid;
		this->fh = fh;
		this->rx0 = rx0;
		this->rx1 = rx1;
		this->flip = flip;
	}
	void setNPCID(int id){
		npcid= id;
	}
	void setRX0(int rx0){
		this->rx0 = rx0;
	}
	void setRX1(int rx1){
		this->rx1 = rx1;
	}
	void setFlip(bool f){
		flip = f;
	}
	int getNPCID(){
		return npcid;
	}
	int getRX0(){
		return rx0;
	}
	int getRX1(){
		return rx1;
	}
	bool getFlip(){
		return flip;
	}
};

#endif