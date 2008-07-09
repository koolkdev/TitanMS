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
};


#endif