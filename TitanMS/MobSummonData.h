#ifndef MOBSUMMONDATA_H
#define MOBSUMMONDATA_H

#include <vector>

class MobSummonData {
private:
	vector<int> mobs;
public:
	MobSummonData(){}

	void add(int id){
		mobs.push_back(id);

	}
	vector<int> getMobs(){
		return mobs;
	}

};

#endif