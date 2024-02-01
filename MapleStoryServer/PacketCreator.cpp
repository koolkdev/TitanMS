 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#include "PacketCreator.h"
#include <stdio.h>
#include "Player.h"
#include "PlayerLogin.h"
#include "MasterServer.h"
#include "Channel.h"

void Packet::addHeader(short headerid){
	(*(short*)packet) = headerid;
}

void Packet::addInt(int intg){
	(*(int*)(packet+pos)) = intg;
	pos+=4;
}

void Packet::addInt64(__int64 int64){
	(*(__int64*)(packet+pos)) = int64;
	pos+=8;
}

void Packet::addShort(short shrt){
	(*(short*)(packet+pos)) = shrt;
	pos+=2;
	
}

void Packet::addByte(unsigned char byte){
	packet[pos++] = byte;
}

void Packet::addBytes(char *hex){
	for(int i=0; i<(int)strlen(hex)/2; i++){
		unsigned char byte1 = hex[i*2];
		unsigned char byte2 = hex[i*2+1];
		if(byte1 >= 'A' && byte1 <= 'F')
			byte1 -= 'A' - 0xa;
		else if(byte1 >= 'a' && byte1 <= 'f')
			byte1 -= 'a' - 0xa;
		else if(byte1 >= '0' && byte1 <= '9')
			byte1 -= '0';
		if(byte2 >= 'A' && byte2 <= 'F')
			byte2 -= 'A' - 0xa;
		else if(byte2 >= 'a' && byte2 <= 'f')
			byte2 -= 'a' - 0xa;
		else if(byte2 >= '0' && byte2 <= '9')
			byte2 -= '0';
		unsigned char byte = byte1*0x10 + byte2;
		packet[pos++] = byte;	
	}
}

void Packet::addString(char *str, int slen){
	int rlen = strlen(str);
	strncpy_s((char*)packet+pos, slen+1, str, slen);
	for(int i=rlen; i<slen; i++)
		packet[pos+i] = 0;
	pos+=slen;
}

void Packet::addBytesHex(unsigned char* bytes, int len){
	memcpy_s(packet+pos, len, bytes, len);
	pos+=len;
}

void Packet::packetSend(Player* player){
	unsigned char tempbuf[10000]; 
	for(int i=0; i<pos; i++){
		tempbuf[i] = packet[i];
	}
	player->sendPacket(tempbuf, pos);
}

void Packet::sendTo(Player* player, vector <Player*> players, bool is){
	for(unsigned int i=0; i<players.size(); i++){
		if((player != NULL && player != players[i] && !is) || is)
			this->packetSend(players[i]);
	}
}

void Packet::packetSendLogin(PlayerLogin* player){
	unsigned char tempbuf[10000];
	for(int i=0; i<pos; i++){
		tempbuf[i] = packet[i];
	}
	player->sendPacket(tempbuf, pos);
}
void Packet::packetSendMasterServer(MasterServer* masterServer){
	unsigned char tempbuf[10000];
	for(int i=0; i<pos; i++){
		tempbuf[i] = packet[i];
	}
	masterServer->sendPacket(tempbuf, pos);
}
void Packet::packetSendChannel(Channel* channel){
	unsigned char tempbuf[10000];
	for(int i=0; i<pos; i++){
		tempbuf[i] = packet[i];
	}
	channel->sendPacket(tempbuf, pos);
}

void Packet::sendToChannels(Channel* channel, Channel* channels[], bool is){
	for(int i=0; i<20; i++){
		if((channels[i] != NULL && channels[i] != channel && !is) || is)
			this->packetSendChannel(channels[i]);
	}
	
}