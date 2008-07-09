#ifndef QUESTSDATA_H
#define QUESTSDATA_H

#include "TopData.h"
#include "Initializing.h"

class QuestData;

class QuestsData : public TopData<QuestsData, QuestData> {
	QuestData* load(int id){
		return Initializing::loadQuest(id);
	}
};


#endif