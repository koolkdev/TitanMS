#ifndef MAPPORTALDATA_H
#define MAPPORTALDATA_H

#include "DataStruct.h"
#include <string>

class MapPortalData : public DataStruct {
private:
	int x;
	int y;
	string from;
	string to;
	int toid;
	int type;
	string script;
public:
	MapPortalData(){
		id=-1;
		x=0;
		y=0;
		from = "";
		to = "";
		toid =0; 
		type =0;
		script = "";
	}

	void setX(int x){
		this->x = x;
	}
	int getX(){
		return x;
	}
	void setY(int y){
		this->y = y;
	}
	int getY(){
		return y;
	}
	void setType(int type){
		this->type = type;
	}
	int getType(){
		return type;
	}
	void setToMapID(int toid){
		this->toid = toid;
	}
	int getToMapID(){
		return toid;
	}
	void setFromPortal(string s){
		from = s;
	}
	string getFromPortal(){
		return from;
	}
	void setToPortal(string s){
		to = s;
	}
	string getToPortal(){
		return to;
	}
	void setScript(string s){	
		script = s;
	}
	string getScript(){
		return script;
	}


};

#endif