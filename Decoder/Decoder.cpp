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

#include "Decoder.h"
#include "AES.h"
#include "MapleEncryption.h"
#include "../TitanMS/Tools.h"
using namespace Tools;

AES* Decoder::aes;
int Decoder::version;

void Decoder::Initialize(int version){
	aes = new AES();
	unsigned char key[32] = {0x13,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0xB4,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x33,0x00,0x00,0x00,0x52,0x00,0x00,0x00};
	aes->SetParameters(256);
	aes->StartEncryption(key);
	Decoder::version = version;
}

void Decoder::encrypt(unsigned char *buffer, int size){
	MapleEncryption::mapleEncrypt(buffer, size);
	int pos=0,first=1;
	while(size > pos){
		if(size>pos+1460-first*4){
			aes->DecryptOFB(buffer+pos, Decoder::ivSend, 1460 - first*4);
		}
		else
			aes->DecryptOFB(buffer+pos, Decoder::ivSend, size-pos);
		pos+=1460-first*4;
		if(first)
			first=0;
	}
} 
 
void Decoder::next(){
	MapleEncryption::nextIV(Decoder::ivSend);
}
 
void Decoder::decrypt(unsigned char *buffer, int size){
	int pos=0,first=1;
	while(size > pos){
		if(size>pos+1460-first*4){
			aes->DecryptOFB(buffer+pos, Decoder::ivRecv, 1460 - first*4);
		}
		else
			aes->DecryptOFB(buffer+pos, Decoder::ivRecv, size-pos);
		pos+=1460-first*4;
		if(first)
			first=0;
	}
	MapleEncryption::nextIV(Decoder::ivRecv); 
	MapleEncryption::mapleDecrypt(buffer, size);
} 

void Decoder::createHeader (unsigned char* header, short size) {
	short a = ivSend[3]*0x100 + ivSend[2];
	a = a ^ -(version+1);
	short b = a ^ size; 
	header[0] = a%0x100;
	header[1] = (a-header[0])/0x100;
	header[2] = b%0x100;
	header[3] = (b-header[2])/0x100;
}
unsigned char* Decoder::getConnectPacket() {
	(*(int*)ivRecv) = randomInt();
	(*(int*)ivSend) = randomInt();
	(*(short*)connectBuffer) = 0x000D;
	(*(int*)(connectBuffer+sizeof(short))) = version;
	memcpy_s(connectBuffer+6, 4, ivRecv, 4);
	memcpy_s(connectBuffer+10, 4, ivSend, 4);
	connectBuffer[14] = 0x08;
	return connectBuffer;
}