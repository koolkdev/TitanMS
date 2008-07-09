#ifndef DATASTRUCT_H
#define DATASTRUCT_H

class DataStruct {
protected:
	int id;
public:
	DataStruct(){}
	DataStruct(int id){
		this->id = id;
	}
	int getID(){
		return id;
	}
	void setID(int id){
		this->id = id;
	}
};

#endif