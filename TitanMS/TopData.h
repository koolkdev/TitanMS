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

#ifndef TOPDATA_H
#define TOPDATA_H

#include <vector>
#include <hash_map>

#include "DataStruct.h"
#include "TopDataStruct.h"

template <typename T, typename D, typename R = int>
class TopData : public TopDataStruct<D, R> {
protected:
	static T* instance;
	stdext::hash_map <R, int> datar;
	virtual D* load(R id) = 0;
public:
	TopData(){
		instance = (T*)this;
	}
	static T* getInstance(){
		if (instance == NULL)instance = new T();
		return instance;
	}
	void registerData(R id){
		datar[id] = true;
	}
	D* getDataByID(R id){
		if(datah.find(id) != datah.end())
			return datah[id];
		if(datar.find(id) != datar.end())
			return load(id);
		return NULL;
	}
	std::vector <D*>* getData(){
		if(datah.size() < datar.size())
			for(hash_map <R, int>::iterator iter = datar.begin(); iter != datar.end(); iter++)
				if(darah.find(iter->first) == datah.end())
					load(iter->first);
		return &datav;
	}
};


#endif