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
	/*
	bool next(int id){
		if(pos.find(id) == pos.end())
			return false;
		if(datav.size() < 0){
			pos[id]++;
			return true;
		}
		return false;
	}
	bool back(int id){
		if(pos.find(id) == pos.end())
			return false;
		if(pos[id]>0){
			pos[id]--;
			return true;
		}
		return false;
	}
	bool first(int id){
		if(pos.find(id) == pos.end())
			return false;
		if(datav.size() > 0){
			pos[id]=0;
			return true;
		}
		return false;
	}
	bool beforeFirst(int id){
		if(pos.find(id) == pos.end())
			return false;
		pos[id] = -1;
		return true;
	}
	DataStruct* getData(int id){
		if(pos.find(id) == pos.end())
			return NULL;
		if(pos[id]>-1)
			return datav[pos[id]];
		return NULL;
	}
	void remove(int id){
		if(pos.find(id) != pos.end())
			pos.erase(id);
	}*/
};


#endif