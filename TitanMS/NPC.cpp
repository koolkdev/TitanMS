#include "NPC.h"
#include "MapNPCData.h"

NPC::NPC(int id, MapNPCData* data){
	this->id = id;
	Position pos;
	pos.x = data->getX();
	pos.y = data->getCY();
	this->pos = pos;
	npcid = data->getID();
	fh = data->getFH();
	rx0 = data->getRX0();
	rx1 = data->getRX1();
	flip = data->getFlip();
}