void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("Are you done with your training? ");
		npc.addText("If you wish, I will send you out from this training camp.");
		npc.sendYesNo();
	}
	else if(state == 1){
		if(npc.getSelected() == 1){
			npc.addText("Then, I will send you out from here. Good job.");
			npc.sendNext();
		}
		else{
			npc.addText("Haven't you finish the training program yet? ");
			npc.addText("If you want to leave this place, please do not hesitate to tell me.");
			npc.sendOK();
		}
	}
	else if(state == 2){
		if(npc.getSelected() == 1){
			npc.getPlayer().changeMap(3);
		}
		npc.end();
	}
}