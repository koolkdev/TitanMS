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
#include "MovingHandlingError.h"
#include "PacketReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

string valstr(int id);

MovingHandlingError::MovingHandlingError(PacketReader* packet, int movetype){
	char packeth[10];
	sprintf_s(packeth, 10, "%x", packet->getHeader());
	error = "Error with handling the packet: 0x";
	error += string(packeth);
	error += ". Can't handle the movement type ";
	error += valstr(movetype);
	error += ". The packet was sent by ";
	error += valstr(packet->getPlayerID());
	error += "\n";
}