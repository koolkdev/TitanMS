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

#ifndef TOPDATASTRUCT_H
#define TOPDATASTRUCT_H

#include <vector>
#include <hash_map>

#include "DataStruct.h"

template <typename T, typename R = int>
class TopDataStruct {
protected:
	stdext::hash_map <R, T*> datah;
	std::vector <T*> datav;
public:
	TopDataStruct(){
	}
	void add(T* ds){
		datah[ds->getID()] = ds;
		datav.push_back(ds);
	}

	T* getDataByID(R id){
		if(datah.find(id) != datah.end())
			return datah[id];
		return NULL;
	}
	int getCount(){
		return (int)datav.size();
	}
	std::vector <T*>* getData(){
		return &datav;
	}
};


#endif