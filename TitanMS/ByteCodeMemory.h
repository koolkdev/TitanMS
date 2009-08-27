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

#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include "AngelScriptEngine.h"
using namespace std;

template <typename T>
class ByteCodeMemory : public asIBinaryStream {
private:
	int readPos, writePos;
	unsigned char* bytes;
	string name;
	int len;
	T id;
public:
	ByteCodeMemory(T id, char* name): bytes(new unsigned char[256]), len(256), readPos(0), writePos(0), name(string(name)), id(id){}
	void adjust(){
		unsigned char* newbytes = new unsigned char[len*2];
		memcpy (newbytes, bytes, len);
		len *= 2;
		delete [] bytes;
		bytes = newbytes;
	}
	void resetReadPos(){
		readPos = 0;
	}
	void resetWritePos(){
		readPos = 0;
	}
	void Write(const void *ptr, asUINT size){
		while (writePos + (int)size > len) {
			adjust();
		}
		memcpy (bytes+writePos, ptr, size);
		writePos+=size;
	}
	void Read(void *ptr, asUINT size){
		memcpy (ptr, bytes+readPos, size);
		readPos+=size;		
	}
	T getID(){
		return id;
	}
	char* getName(){
		return (char*)name.c_str();
	}

};

#endif
