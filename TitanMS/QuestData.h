#ifndef QUESTDATA_H
#define QUESTDATA_H

#include "TopDataStruct.h"
#include "QuestRewardsData.h"
#include "QuestRequestsData.h"

class QuestData : public DataStruct {
private:
	QuestRewardsData* rewards;
	QuestRequestsData* requests;
	int nextQuest;
public:
	QuestData(int id){
		this->id = id;
		rewards = new QuestRewardsData();
		requests = new QuestRequestsData();
	}

	QuestRewardsData* getRewardsData(){
		return rewards;
	}
	QuestRequestsData* getRequestsData(){
		return requests;
	}
	void setNextQuest(int nextQuest){
		this->nextQuest = nextQuest;
	}
	int getNextQuest(){
		return nextQuest;
	}
};


#endif