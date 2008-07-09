#ifndef SHOPSDATA_H
#define SHOPSDATA_H

#include "TopData.h"
#include "Initializing.h"

class ShopData;

class ShopsData : public TopData<ShopsData, ShopData> {
	ShopData* load(int id){
		return Initializing::loadShop(id);
	}
};



#endif