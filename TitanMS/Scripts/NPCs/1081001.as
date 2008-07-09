/* Made by ShingingDB */
void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		npc.addText("So you want to leave #bFlorina Beach#k? If you want, I can take you back to #bLith Harbor#k. \r\n#L0# #bI would like to go back now.#k#l");
		npc.sendSimple();
	}
	else if(state == 1){
		npc.addText("Are you sure you want to return to #bLith Harbor#k? Alright, we'll have to get going fast. Do you want to head back to Lith Harbor now?");
		npc.sendYesNo();
	}
	else if(state == 2){
		if(npc.getSelected() == YES){
			npc.getPlayer().changeMap(104000000);
			npc.end();
		}
		else{
			npc.addText("You must have some business to take care of here. It's not a bad idea to take some rest at Lith Harbor. Look at me, I love it so much here that I wound up living here. Hahaha, anyway, talk to me when you feel like going back.");
			npc.sendNext();
			npc.end();
		}
	}

}