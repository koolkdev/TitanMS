void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		if(player.getJob() == 300){
			if(player.getLevel() >= 30){
				if(player.getItemAmount(4031012) == 0){
					if(player.getItemAmount(4031010) > 0){
						npc.addText("Ah yes this is Athena's handwriting. She must have great faith in you. So are you ready to take the test?");
						state = 1;
						npc.sendYesNo();
					}
					else{
						npc.addText("You can't take the test without a recommendation.");
						npc.sendOK();
						npc.end();
					}
				}
				else{
					npc.addText("What are you waiting for? Take that medal to Athena.");
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
			npc.addText("Wonderful day isn't it?");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			npc.addText("Very well. Let the test commence.");
			player.giveItem(4031010, -1);
			player.changeMap(108000100);
			npc.end();
		}
		else{
			npc.addText("I see.  Come back when you are ready to take the test.");
			npc.sendOK();
			npc.end();
		}
	}
}