#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "Tools.h"

class MapObject {
protected:
	Position pos;
	int id;
public:
	Position getPosition(){
		return pos;
	}
	void setPosition (Position pos){
		this->pos = pos;
	}
	int getID(){
		return id;
	}
	void setID(int id){
		this->id = id;
	}

};

#endif