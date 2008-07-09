#include "Character.h"
#include "CharacterEquip.h"

Character::~Character(){
	for(int i=0; i<(int)equips.size(); i++){
		delete equips[i];
	}
}		