#ifndef ObjectMoving_H
#define ObjectMoving_H

#include "Tools.h"
class PacketReader;
#include "PacketWriter.h"

class ObjectMoving{
private:
	Position pos;
	PacketWriter pw;
	int stance;
	bool change;
	bool error;
public:
	ObjectMoving(PacketReader *packet);
	Position getPosition(){
		return pos;
	}
	PacketWriter* getPacket(){
		return &pw;
	}
	int getStance(){
		return stance;
	}
	bool isChange(){
		return change;
	}
	bool isError(){
		return error;
	}

};

#endif