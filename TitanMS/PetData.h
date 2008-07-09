#ifndef PETDATA_H
#define PETDATA_H

#include "DataStruct.h"
#include "TopDataStruct.h"
using namespace std;
class PetCommandData;

class PetData : public DataStruct, public TopDataStruct<PetCommandData> {
private:
	int hungry;
	int nameTag;
	int chatBalloon;
	string name;
	int life;
public:
	PetData(int id){
		this->id = id;
		hungry=0;
		nameTag=0;
		chatBalloon=0;
		name="";
		life=0;
	}

	void setHungry(int hungry){
		this->hungry = hungry;
	}
	void setNameTag(int nameTag){
		this->nameTag = nameTag;
	}
	void setChatBalloon(int chatBalloon){
		this->chatBalloon = chatBalloon;
	}
	void setName(string name){
		this->name = name;
	}
	void setLife(int life){
		this->life = life;
	}
	int getHungry(){
		return hungry;
	}
	int getNameTag(){
		return nameTag;
	}
	int getChatBalloon(){
		return chatBalloon;
	}
	string getName(){
		return name;
	}
	int getLife(){
		return life;
	}
};

#endif