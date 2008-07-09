 /*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "PacketWriter.h"
#include "BytesStream.h"
#include <stdio.h>
using namespace std;

void PacketWriter::writeInt(int intg){
	stream.add((int)intg);
}

void PacketWriter::writeLong(__int64 int64){
	stream.add((__int64)int64);
}

void PacketWriter::writeShort(short shrt){
	stream.add((short)shrt);
	
}

void PacketWriter::write(unsigned char byte){
	stream.add((char)byte);
}

void PacketWriter::writeBytes(char *hex){
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
		stream.add((char)byte);
	}
}

void PacketWriter::writeString(string str, int slen){
	int rlen = str.length();
	stream.add(str);
	for(int i=rlen; i<slen; i++)
		stream.add((char)0);
}
void PacketWriter::writeString(string str){
	stream.add((short)str.length());
	stream.add(str);
}
void PacketWriter::writeString(char* str, int slen){
	int rlen = strlen(str);
	stream.add(string(str));
	for(int i=rlen; i<slen; i++)
		stream.add((char)0);
}
void PacketWriter::writeString(char* str){
	stream.add((short)strlen(str));
	stream.add(string(str));
}

void PacketWriter::write(unsigned char* bytes, int len){
	stream.add(bytes, len);
}
void PacketWriter::show(){
	stream.show();
}

unsigned char* PacketWriter::getBytes(){
	return stream.getBytes();
}
int PacketWriter::getLength(){
	return stream.getLength();
}
/*
void Packet::packetSend(Player* player){
	unsigned char tempbuf[10000]; 
	//if(packet[0] == 0xa5)
		printf("\n");
	for(int i=0; i<pos; i++){
	//	if(packet[0] == 0xa5)
			printf("%x ", packet[i]);
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
void Packet::packetSendChannel(ChannelTemp* channel){
	unsigned char tempbuf[10000];
	for(int i=0; i<pos; i++){
		tempbuf[i] = packet[i];
	}
	channel->sendPacket(tempbuf, pos);
}

void Packet::sendToChannels(ChannelTemp* channel, ChannelTemp* channels[], bool is){
	for(int i=0; i<20; i++){
		if((channels[i] != NULL && channels[i] != channel && !is) || is)
			this->packetSendChannel(channels[i]);
	}
	
}*/