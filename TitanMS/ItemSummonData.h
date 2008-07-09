#ifndef ITEMSUMMONDATA_H
#define ITEMSUMMONDATA_H

#include <vector>
using namespace std;

class ItemSummonData {
public:
	static struct SummonData {
		int id;
		int chance;
	};
	ItemSummonData(){}

	void add(int id, int chance){
		SummonData sd;
		sd.id = id;
		sd.chance = chance;
		mobs.push_back(sd);
	}
private:
	vector<SummonData> mobs;

};

#endif