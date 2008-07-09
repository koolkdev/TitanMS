#ifndef SHOPDATA_H
#define SHOPDATA_H

#include "TopDataStruct.h"
class ShopItemData;

class ShopData : public TopDataStruct<ShopItemData>, public DataStruct {
private:
	int npc;
public:
	ShopData(int id){
		this->id = id;
		npc=0;
	}
	
	void setNPC(int npc){
		this->npc = npc;
	}
	int getNPC(){
		return npc;
	}
};


#endif