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

#ifndef HANDLER_H
#define HANDLER_H

#include <hash_map>
class PacketReader;

template <class T>
class Handler {
public:
	typedef void (T::*handlerf)(PacketReader& packet);
	void handle(PacketReader& packet){
		if(handlers.find(packet.getHeader()) == handlers.end()){
			if(handlers.find(-1) != handlers.end())
				((T*)this->*handlers[-1])(packet);
#ifdef _DEBUG
			else
				printf("Unk header: %x\n", packet.getHeader());
#endif
		}
		else{
			handlerf func = handlers[packet.getHeader()];
			((T*)this->*func)(packet);
		}
	}
	static void loadPackets() {};
protected:
	static stdext::hash_map <short, handlerf> handlers;
};
template <class T> void Handler<T>::handle(PacketReader& packet);

#endif