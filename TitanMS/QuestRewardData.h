#ifndef QUESTREWARDDATA_H
#define QUESTREWARDDATA_H

#include "DataStruct.h"

class QuestRewardData : public DataStruct {
private:
	bool start;
	bool item;
	bool quest;
	bool mesos;
	bool exp;
	bool fame;
	int count;
	int gender;
	int job;
	int prop;
public:
	QuestRewardData(){
		id=-1;
		start = 0;
		item = 0;
		mesos = 0;
		exp = 0;
		fame = 0;
		count = 0;
		gender = 0;
		job = 0;
		prop = 0;
	}

	void setStart(bool start){
		this->start = start;
	}
	bool getStart(){
		return start;
	}
	void setItem(bool item){
		this->item = item;
	}
	bool getItem(){
		return item;
	}
	void setMesos(bool mesos){
		this->mesos = mesos;
	}
	bool getMesos(){
		return mesos;
	}
	void setExp(bool exp){
		this->exp = exp;
	}
	bool getExp(){
		return exp;
	}
	void setFame(bool fame){
		this->fame = fame;
	}
	bool getFame(){
		return fame;
	}
	void setCount(int count){
		this->count = count;
	}
	int getCount(){
		return count;
	}
	void setGender(int gender){
		this->gender = gender;
	}
	int getGender(){
		return gender;
	}
	void setJob(int job){
		this->job = job;
	}
	int getJob(){
		return job;
	}
	void setProp(int prop){
		this->prop = prop;
	}
	int getProp(){
		return prop;
	}

};

#endif