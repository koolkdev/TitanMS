#ifndef EQUIPSDATA_H
#define EQUIPSDATA_H

#include "TopData.h"
#include "Initializing.h"

class EquipData;

class EquipsData : public TopData<EquipsData, EquipData> {
	EquipData* load(int id){
		return Initializing::loadEquip(id);
	}
};


#endif