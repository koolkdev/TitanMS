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
#include "PacketCreator.h"
#include "PacketWriter.h"
#include "Quest.h"

PacketWriter* PacketCreator::updateQuest(short questid, int npcid, int nextquest){
	pw.writeShort(QUEST_UPDATE);

	pw.write(8);
	pw.writeShort(questid);
	pw.writeInt(npcid);
	pw.writeInt(nextquest);

	return &pw;
}
PacketWriter* PacketCreator::updateQuestInfo(Quest* quest, bool forfeit){
	pw.writeShort(SHOW_GAIN);

	pw.write(1);
	pw.writeShort(quest->getID());
	pw.write(forfeit ? 0 : (quest->isCompleted() ? 2 : 1));
	if(!forfeit && quest->isCompleted()){
		pw.writeLong(quest->getCompleteTime());
	}
	else{
		if(!forfeit) pw.writeString(quest->getKilledMobs());
		else pw.writeString("");
		pw.writeLong(0);
	}

	return &pw;
}
PacketWriter* PacketCreator::questDone(int questid){
	pw.writeShort(COMPLETE_QUEST);

	pw.writeShort(questid);

	return &pw;
}
PacketWriter* PacketCreator::itemGainChat(int itemid, int amount){
	pw.writeShort(ITEM_GAIN_CHAT);
	
	pw.write(3);
	pw.write(1); // TODO: items num
	pw.writeInt(itemid);
	pw.writeInt(amount);

	return &pw;

}
PacketWriter* PacketCreator::mesosGainChat(int amount){
	pw.writeShort(SHOW_GAIN);
	
	pw.write(5);
	pw.writeInt(amount);

	return &pw;
}