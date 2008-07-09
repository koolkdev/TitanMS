#ifndef REACTORSCRIPTSDATA_H
#define REACTORSCRIPTSDATA_H

#include "TopData.h"
#include "AngelScriptEngine.h"
#include "ByteCodeMemory.h"

class ReactorScriptsData : public TopData<ReactorScriptsData, ByteCodeMemory<int>> {
	ByteCodeMemory<int>* load(int id){
		return AngelScriptEngine::loadReactor(id);
	}
};



#endif