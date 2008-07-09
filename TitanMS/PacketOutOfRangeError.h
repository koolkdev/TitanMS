#ifndef PACKETOUT_H
#define PACKETOUT_H

#include "PacketHandlingError.h"

class PacketReader;
class Player;

class PacketOutOfRangeError : public PacketHandlingError {
public:
	PacketOutOfRangeError(PacketReader* packet, int bytes);
};

#endif