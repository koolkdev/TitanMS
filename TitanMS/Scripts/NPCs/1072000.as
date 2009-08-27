void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		if(player.getJob() == 100){
			if(player.getLevel() >= 30){
				if(player.getItemAmount(4031012) == 0){
					if(player.getItemAmount(4031008) > 0){
						npc.addText("I see, so Dances with Balrogs wishes for me to test you. Well then, are you ready to begin the test?");
						npc.sendYesNo();
					}
					else{
						npc.addText("You must have a recommendation from Dances with Balrogs to take this test.");
						npc.sendOK();
						npc.end();
					}
				}
				else{
					npc.addText("You're still here? Hurry up and take that to Dances with Balrogs.");
					npc.sendOK();
					npc.end();
				}
			}
			else{
				npc.addText("You are too weak to advance.  Come back when you are at least level 30.");
				npc.sendOK();
				npc.end();
			}
		}
		else{
			npc.addText("Hello traveller.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			npc.addText("Very well. Let the test commence.");
			player.giveItem(4031008, -1);
			player.changeMap(108000300);
			npc.end();
		}	
		else{
			npc.addText("I see.  Come back when you are ready to take the test.");
			npc.sendOK();
			npc.end();
		}
	}
}
