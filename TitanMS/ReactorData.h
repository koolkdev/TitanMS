#ifndef REACTOR_DATA
#define REACTOR_DATA

#include "DataStruct.h"
#include "TopDataStruct.h"
using namespace std;
class ReactorStateData;

class ReactorData: public DataStruct, public TopDataStruct<ReactorStateData> {
private:
	string action;
public:
	ReactorData(int id){
		this->id = id;
		action = "";
	}

	void setAction(string a){
		action = a;
	}
	string getAction(){
		return action;
	}
};

#endif