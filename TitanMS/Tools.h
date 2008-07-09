#ifndef TOOLS_H
#define TOOLS_H

struct Position {
	short x;
	short y;
};

#include <vector>

class Value {
	__int64 type;
	int value;
public:
	Value(__int64 type, int value){
		this->type = type;
		this->value = value;
	}
	__int64 getType(){
		return type;
	}
	int getValue(){
		return value;
	}
};

class Values {
protected:
	int pos;
	std::vector <Value> values;
public:
	Values(){
		pos = -1;
	}
	void add(Value value){
		values.push_back(value);
	}/*
	void beforeFirst(){
		pos=-1;
	}
	bool next(){
		if(pos + 1 < (int)values.size()){
			pos++;
			return true;
		}
		return false;
	}
	Value* getValues(){
		if(pos == -1 || pos >= (int)values.size()){
			if(values.size() == 0)
				return NULL;
			return &values[0];
		}
		return &values[pos];
	}*/
	std::vector <Value>* getValues(){
		return &values;
	}
	void sort(){
		int n = values.size();
		for(int i=0; i<n; i++){
			for(int j=0; j<n-1-i; j++){
				if(values[j].getType() > values[j+1].getType()){
					Value temp = values[j];
					values[j] = values[j+1];
					values[j+1] = temp;
				}
			}
		}
	}
	int getSize(){
		return values.size();
	}
	int getValueByType(__int64 type){
		for(int i=0; i<(int)values.size(); i++){
			if(values[i].getType() == type)
				return values[i].getValue();
		}
		return 0;

	}
	bool contain(__int64 type){
		for(int i=0; i<(int)values.size(); i++){
			if(values[i].getType() == type)
				return true;
		}
		return false;
	}
};

namespace Tools {
	int getDistance(Position pos1, Position pos2);
	void randomize();
	int randomInt();
	int random(int min, int max);
	int random(int n);
	int getTime();
	__int64 getLongTime();
	__int64 doubleAsLong(double num);
	__int64 getStatsType(Values* values);
	__int64 getTimeByDays(int days);
};
#endif