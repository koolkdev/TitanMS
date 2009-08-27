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

#ifndef BYTESSTREAM_H
#define BYTESSTREAM_H

#include <string>

#define MAX_LENGTH 32000

class BytesStream {
private:
	unsigned char bytes [MAX_LENGTH];
	int insert;
public:
	BytesStream (): insert(0) {}
	void add (unsigned char* v, int len) {
		if(insert+len < MAX_LENGTH){
			memcpy (bytes+insert, v, len);
			insert+=len;
		}
	}
	void add (char v) {
		add((unsigned char*)&v, sizeof(char));
	}
	void add (short v) {
		add((unsigned char*)&v, sizeof(short));
	}
	void add (int v) {
		add((unsigned char *)&v, sizeof(int));
	}
	void add (__int64 v) {
		add((unsigned char *)&v, sizeof(__int64));
	}
	void add (std::string v) {
		add((unsigned char *)v.data(), v.length());
	}
	unsigned char* getBytes(){
		return bytes;
	}
	int getLength(){
		return insert;
	}
	void show(){
		for(int i=0; i<insert; i++){
			printf("%x ", bytes[i]);
		}
		printf("\n");
	}
};

#endif