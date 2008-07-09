#ifndef NPCSDATA_H
#define NPCSDATA_H

#include "TopData.h"
#include "AngelScriptEngine.h"
#include "ByteCodeMemory.h"

class NPCsData : public TopData<NPCsData, ByteCodeMemory<int>> {
	ByteCodeMemory<int>* load(int id){
		return AngelScriptEngine::loadNPC(id);
	}
};



#endif