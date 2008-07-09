#include "PacketOutOfRangeError.h"
#include "PacketReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

string valstr(int id){
	string ret;
	stringstream buf;
	buf << id;
	buf >> ret;
	return ret;
}

PacketOutOfRangeError::PacketOutOfRangeError(PacketReader* packet, int bytes){
	char packeth[10];
	sprintf_s(packeth, 10, "%x", packet->getHeader());
	error = "Error with handling the packet: 0x";
	error += string(packeth);
	error += ". PacketReader tried to read data that is out of range. Tried to read ";
	error += valstr(bytes);
	error += " bytes at ";
	error += valstr(packet->getPos());
	error += " when the length of the packet is ";
	error += valstr(packet->getSize());
	error += ". Player that sent the packet: ";
	error += valstr(packet->getPlayerID());
	error += "\n";
}