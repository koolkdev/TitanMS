#include "ObjectMoving.h"
#include "PacketReader.h"
#include "PacketWriter.h"

ObjectMoving::ObjectMoving(PacketReader *packet){
	change = false;
	error = false;
	pw = PacketWriter();
	int num = packet->read();
	pw.write((char)num);
	for(int i=0; i<num; i++){
		int type = packet->read();
		pw.write((char)type);
		switch(type){
			case 0:
			case 5:
				{
				pos.x = packet->readShort();
				pos.y = packet->readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeInt(packet->readInt());
				pw.writeShort(packet->readShort());
				stance = packet->read();
				pw.write((char)stance);
				pw.writeShort(packet->readShort());
				change = true;
				break;
			}
			case 1:
			case 2:
			case 6:	{
				pos.x = packet->readShort();
				pos.y = packet->readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				stance = packet->read();
				pw.write((char)stance);
				pw.writeShort(packet->readShort());
				change = true;
				break;
			}
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:	{
				pos.x = packet->readShort();
				pos.y = packet->readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeInt(packet->readInt());
				stance = packet->read();
				pw.write((char)stance);
				break;
			}
			case 10:{
				pw.write(packet->read());
				break;
			}
			case 11:{
				pos.x = packet->readShort();
				pos.y = packet->readShort();	
				pw.writeShort(pos.x);
				pw.writeShort(pos.y);
				pw.writeShort(packet->readShort());
				stance = packet->read();
				pw.write((char)stance);
				pw.writeShort(packet->readShort());
				break;
			}
			default: error = true; printf("Error handling movement: %d", type);
		}
	}
}