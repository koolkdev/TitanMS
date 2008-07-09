#include "Damage.h"
#include "PacketReader.h"

Damage::Damage(PacketReader *packet, bool range){
	packet->read();
	info = packet->read();
	mobCount = info/0x10;
	attackCount = info%0x10;
	skill = packet->readInt();
	packet->read();
	stance = packet->read();

	// Mesos Explosion - TODO
	mesoexplosion = (skill == 4211006);
	if(mesoexplosion)
		return;

	packet->read(6);

	if(range){
		itemslot = packet->readInt();
		packet->read();
	}
	else itemslot = 0;
	for(int i=0; i<mobCount; i++){
		int mobid = packet->readInt();
		packet->read(14);
		mobs.push_back(mobid);
		vector<int> damage;
		for(int j=0; j<attackCount; j++){
			damage.push_back(packet->readInt());
		}
		damages[mobid] = damage;
	}
}