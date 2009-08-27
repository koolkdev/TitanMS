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

#include "MasterDecoder.h"

string MasterDecoder::pass;

void MasterDecoder::encrypt(unsigned char *buffer, int size){
	string tpass = string(pass);; 
	char* key = (char*)tpass.c_str();
	int len = strlen(key);
	char last = 'a';
	for(int j=0; j<3; j++){
		for(int i=size-1; i>=0; i--){
			key[len-i%len-1] += last;
			buffer[i] ^= key[len-i%len-1];
			last -= key[len-i%len-1];
		}
		for(int i=0; i<size; i++){
			key[i%len] ^= (last-4);
			buffer[i] ^= key[i%len];
			last ^= (key[i%len]+8);
		}
	}
} 

void MasterDecoder::decrypt(unsigned char *buffer, int size){
	encrypt(buffer, size);
}

void MasterDecoder::createHeader (unsigned char* header, short size) {
	(*(short*)header) = size;
}