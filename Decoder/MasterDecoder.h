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

#ifndef MASTERDECODER_H
#define MASTERDECODER_H

#define _CRT_RAND_S
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <string>
using namespace std;

class MasterDecoder {
private:
	static string pass;
	unsigned char ivRecv[4];
	unsigned char ivSend[4];
	unsigned char connectBuffer[15];

public:
	static void Initialize(string pass){
		MasterDecoder::pass = pass;
	}
	static int getLength (unsigned char* header) {
		return ((header[0] + header[1]*0x100)); 
	}
	void createHeader (unsigned char* header, short size); 
	void encrypt (unsigned char *buffer, int size);
	void decrypt (unsigned char *buffer, int size);

};

#endif