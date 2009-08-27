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

#ifndef REMOTE_CONTROL
#define REMOTE_CONTROL

#include "../Connection/AbstractPlayer.h"

class PacketWriter;
template <class T> class Handler;
class RemoteControlHandler;

class RemoteControl : public AbstractPlayer {
public:
	RemoteControl ();
	~RemoteControl();
	void handleRequest(unsigned char* buf, int len);
	void send(PacketWriter* packet);
	bool isCorrect(){
		return correct;
	}
	void setCorrect(bool b){
		correct = b;
	}
private:
	bool correct;
	Handler<RemoteControlHandler>* handler;

};

class RemoteControlFactory : public AbstractPlayerFactory {
public:
	AbstractPlayer* createPlayer(int port) {
		return new RemoteControl();
	}
};


#endif