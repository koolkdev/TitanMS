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

#ifndef PACKETWRITER_H
#define PACKETWRITER_H

#include <string>

#include "BytesStream.h"

class PacketWriter {
private:
	BytesStream stream;
public:
	void writeInt(int intg);
	void writeLong(__int64 int64);
	void writeShort(short shrt);
	void writeString(std::string str, int slen);
	void writeString(std::string str);
	void writeString(char* str, int slen);
	void writeString(char* str);
	void write(unsigned char byte);
	void writeBytes(char* hex);
	void write(unsigned char* bytes, int len);
	void show();
	unsigned char* getBytes();
	int getLength();
};

#endif