#include "PlayerKeys.h"
#include "Key.h"

void PlayerKeys::setKey(int n, Key* key){
	if(n >= 0 && n<90){
		if(keys[n] != NULL)
			delete keys[n];
		keys[n] = key;
	}
}
void PlayerKeys::deleteAll(){
	for(int i=0; i<90; i++)
		if(keys[i] != NULL)
			delete keys[i];
}