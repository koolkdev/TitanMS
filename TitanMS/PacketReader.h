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

#ifndef PACKETREADER_H
#define PACKETREADER_H

#include <stdlib.h>
#include <string>
#include "PacketOutOfRangeError.h"
using namespace std;
//?
#define MAX_LEN 10000
class PacketReader {
private:
	int playerid;
	unsigned char* buf;
	short header;
	int length;
	int pos;
public:
	PacketReader(unsigned char* buf, int len, int playerid){
		this->playerid = playerid;
		length = len-2;
		if(length > MAX_LEN) throw "Too big packet\n";
		header = buf[0] + buf[1]*0x100;
		pos = 0;
		this->buf = buf+2;
	}
	char read(){
		if(pos + 1 > length)
			throw PacketOutOfRangeError(this, 1);
		return (char)buf[pos++];
	}
	unsigned char* read(int size){
		if(pos + size > length)
			throw PacketOutOfRangeError(this, size);
		int oldpos = pos;
		pos+=size;
		return buf+oldpos;
	}
	string readString(int len){
		if(pos + len > length)
			throw PacketOutOfRangeError(this, len);
		char temp [MAX_LEN];
		strncpy_s(temp, len+1, (const char*)buf+pos, len); 
		pos+=len;
		string r = string(temp);
		return r;
	}
	string readString(){
		short len = readShort();
		if(pos + len > length)
			throw PacketOutOfRangeError(this, len);
		if(len > MAX_LEN)
			return string("");
		char temp [MAX_LEN];
		strncpy_s(temp, len+1, (const char*)buf+pos, len); 
		pos+=len;
		string r = string(temp);
		return r;
	}
	short readShort(){
		if(pos + 2 > length)
			throw PacketOutOfRangeError(this, 2);
		pos+=2;
		return buf[pos-2] + buf[pos-1]*0x100;

	}
	int readInt(){
		if(pos + 4 > length)
			throw PacketOutOfRangeError(this, 4);
		pos+=4;
		return buf[pos-4] + buf[pos-3]*0x100 + buf[pos-2]*0x100*0x100 + buf[pos-1]*0x100*0x100*0x100;
	}
	short getHeader(){
		return header;
	}
	int getPos(){
		return pos;
	}
	int getSize(){
		return length;
	}
	int getPlayerID(){
		return playerid;
	}
	void show(){
		printf("%x %x ", header, 0);
		for(int i=0; i<length; i++)
			printf("%x ", buf[i]);
		printf("\n");
	}

}; 

#endif