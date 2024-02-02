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

#ifndef REACTOR_DATA
#define REACTOR_DATA

#include "DataStruct.h"
#include "TopDataStruct.h"

class ReactorStateData;

class ReactorData: public DataStruct, public TopDataStruct<ReactorStateData> {
private:
	std::string action;
public:
	ReactorData(int id){
		this->id = id;
		action = "";
	}

	void setAction(std::string a){
		action = a;
	}
	std::string getAction(){
		return action;
	}
};

#endif