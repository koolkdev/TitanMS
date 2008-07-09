#ifndef REACTORSTATEDATA_H
#define REACTORSTATEDATA_H

#include "Tools.h"
#include "DataStruct.h"

class ReactorStateData : public DataStruct {
private:
	int type;
	int nextState;
	int itemid, amount;
	Position rb,lt;
public:
	ReactorStateData(){
		id = -1;
		type = 0;
		nextState = 0;
		itemid =0;
		amount =0;
	}
	void setType(int t){
		type = t;
	}
	void setNextState(int nt){
		nextState = nt;
	}
	void setRB(Position r){
		rb = r;
	}
	void setLT(Position l){
		lt = l;
	}
	void setRBX(int x){
		rb.x = x;
	}
	void setRBY(int y){
		rb.y = y;
	}
	void setLTX(int x){
		lt.x = x;
	}
	void setLTY(int y){
		lt.y = y;
	}
	int getType(){
		return type;
	}
	int getNextState(){
		return nextState;
	}
	Position getRB(){
		return rb;
	}
	Position getLT(){
		return lt;
	}
	void setItemID(int id){
		itemid = id;
	}
	int getItemID(){
		return itemid;
	}
	void setAmount(int a){
		amount = a;
	}	
	int getAmount(){
		return amount;
	}

};

#endif