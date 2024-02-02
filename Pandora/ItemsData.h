#ifndef ITEMSDATA_H
#define ITEMSDATA_H

#include "TopData.h"
#include "Initializing.h"

class ItemData;

class ItemsData : public TopData<ItemsData, ItemData> {
	ItemData* load(int id){
		return Initializing::loadItem(id);
	}
};

#endif