#include "Decoder.h"
#include "AESEncryption.h"
#include "MapleEncryption.h"

void Decoder::encrypt(unsigned char *buffer, int size){
	MapleEncryption::mapleEncrypt(buffer, size);
	decryptofb(buffer, Decoder::ivSend, size);
} 

void Decoder::next(){
	MapleEncryption::nextIV(Decoder::ivSend);
}

void Decoder::decrypt(unsigned char *buffer, int size){
	decryptofb(buffer, Decoder::ivRecv, size);
	MapleEncryption::nextIV(Decoder::ivRecv);
	MapleEncryption::mapleDecrypt(buffer, size);
}

void Decoder::createHeader (unsigned char* header, short size) {
	short a = ivSend[3]*0x100 + ivSend[2];
	a = a ^ -53;
	short b = a ^ size;
	header[0] = a%0x100;
	header[1] = (a-header[0])/0x100;
	header[2] = b%0x100;
	header[3] = (b-header[2])/0x100;
}