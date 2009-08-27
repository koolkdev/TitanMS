#include "AbstractPlayer.h"
#include "PacketHandler.h"

void AbstractPlayer::sendPacket(unsigned char* buf, int len){ packetHandler->sendPacket(buf,len); }