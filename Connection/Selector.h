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

#ifndef SELECTOR_H
#define SELECTOR_H

#include <Winsock2.h>
#include <hash_map>

using namespace stdext;

class Selector {
public:

	class SelectHandler {
	protected:
		bool bend;
	public:
		SelectHandler(){
			bend = false;
		}
		virtual int handle (Selector* selector, int socket) = 0;
		bool isEnd(){ return bend; }
		void end(){ bend = true;}
	};

	Selector ();
	~Selector();

	void registerSocket 
		(int socket, 
		bool selectRead, bool selectWrite, bool selectError, 
		SelectHandler* handler);

	void unregisterSocket (int socket);
	void selectThread();

private:
	bool terminate;
	fd_set readfds;
    fd_set writefds;
	fd_set errorfds;
	struct timeval timeout;
	hash_map<int, SelectHandler*> handlers;
};

#endif