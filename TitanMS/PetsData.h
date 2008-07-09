#ifndef PETSDATA_H
#define PETSDATA_H

#include "TopData.h"
#include "Initializing.h"
class PetData;

class PetsData : public TopData<PetsData, PetData> {
	PetData* load(int id){
		return Initializing::loadPet(id);
	}
};

#endif