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

#ifndef MAPNPCDATA_H
#define MAPNPCDATA_H

#include "DataStruct.h"

class MapNPCData : public DataStruct {
private:
	int x;
	int cy;
	int fh;
	int rx0;
	int rx1;
	bool flip;
public:
	MapNPCData(){
		id=-1;
		x = 0;
		cy = 0;
		fh = 0;
		rx0 = 0;
		rx1 = 0;
		flip = 0;
	}

	void setX(int x){
		this->x = x;
	}
	int getX(){
		return x;
	}
	void setCY(int cy){
		this->cy = cy;
	}
	int getCY(){
		return cy;
	}
	void setFH(int fh){
		this->fh = fh;
	}
	int getFH(){
		return fh;
	}
	void setRX0(int rx0){
		this->rx0 = rx0;
	}
	int getRX0(){
		return rx0;
	}
	void setRX1(int rx1){
		this->rx1 = rx1;
	}
	int getRX1(){
		return rx1;
	}
	void setFlip(bool f){
		flip = f;
	}
	bool getFlip(){
		return flip;
	}

};

#endif