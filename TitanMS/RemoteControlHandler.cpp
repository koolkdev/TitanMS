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

#include "DataProvider.h"
#include "RemoteControlHandler.h"
#include "RemoteControl.h"
#include "PacketReader.h"
#include "PacketCreator.h"
#include "Worlds.h"
using namespace stdext;

hash_map <short, RemoteControlHandler::handlerf> Handler<RemoteControlHandler>::handlers;

void RemoteControlHandler::loadPackets(){
	handlers[LOGIN] = &RemoteControlHandler::loginHandle;
	handlers[COMMAND] = &RemoteControlHandler::commandHandle;
	handlers[LOGIN_FAIL] = &RemoteControlHandler::loginFailedHandle;
}

void RemoteControlHandler::loginHandle(PacketReader& packet){
	if(packet.readString() == Worlds::getInstance()->getPassword()){
		remote->send(PacketCreator().loginReplay(true));
		remote->setCorrect(true);
	}
	else{
		remote->send(PacketCreator().loginReplay(false));
	}
}
void RemoteControlHandler::loginFailedHandle(PacketReader& packet){
	remote->send(PacketCreator().loginReplay(false));
}
void RemoteControlHandler::commandHandle(PacketReader& packet){
	if(!remote->isCorrect()) remote->send(PacketCreator().loginReplay(false));
	string rep = Worlds::getInstance()->handleCommand(packet.readString());
	if(rep != "")
		remote->send(PacketCreator().commandReplay(rep));
}