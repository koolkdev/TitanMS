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
#include "ObjectMoving.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "MovingHandlingError.h"

ObjectMoving::ObjectMoving(PacketReader& packet, LifeMapObject* obj){
	pw = PacketWriter();
	int num = packet.read();
	pos = obj->getPosition();
	stance = obj->getStance();
	fh = obj->getFoothold();
	pw.write((char)num);
	for(int i=0; i<num; i++){
		int type = packet.read();
		pw.write((char)type);
		switch(type){
			case 0:
			case 5:
				{
				pos.x = packet.readShort();
				pos.y = packet.readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeInt(packet.readInt());
				fh = packet.readShort();
				pw.writeShort(fh);
				stance = packet.read();
				pw.write((char)stance);
				pw.writeShort(packet.readShort());
				break;
			}
			case 1:
			case 2:
			case 6:	{
				pos.x = packet.readShort();
				pos.y = packet.readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				stance = packet.read();
				pw.write((char)stance);
				pw.writeShort(packet.readShort());
				break;
			}
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:	{
				pos.x = packet.readShort();
				pos.y = packet.readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeInt(packet.readInt());
				stance = packet.read();
				pw.write((char)stance);
				break;
			}
			case 10:{
				pw.write(packet.read());
				break;
			}
			case 11:{
				pos.x = packet.readShort();
				pos.y = packet.readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				fh = packet.readShort();
				pw.writeShort(fh);
				stance = packet.read();
				pw.write((char)stance);
				pw.writeShort(packet.readShort());
				break;
			}
			case 14:{
				pw.write(packet.read(9), 9); // "00 00 00 00 00 00 0A 00 00"
				break;
			}
			case 15:{
				pos.x = packet.readShort();
				pos.y = packet.readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeShort(packet.readShort());
				pw.writeShort(packet.readShort()); //?
				pw.writeShort(packet.readShort());
				fh = packet.readShort();
				pw.writeShort(fh);
				stance = packet.read();
				pw.write((char)stance);
				pw.writeShort(packet.readShort());
				break;
			}
			default: throw MovingHandlingError(&packet, type);
		}
	}
	obj->setPosition(pos);
	obj->setStance(stance);
	obj->setFoothold(fh);
}