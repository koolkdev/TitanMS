void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();	
	if(state == 0){
		npc.addText("Take this ship and you'll head off to a bigger continent.#e For 150 mesos#n, I'll take you to #bVictoria Island#k");
		npc.addText(". The thing is, once you leave this place, you can't ever come back. What do you think? Do you want to go to Victoria Island?");
		npc.sendYesNo();
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			npc.addText("Bored of this place? Here... Give me 150 mesos first...");
			npc.sendNext();
		}
		else{
			npc.addText("Hmm... I guess you still have things to do here?");
			npc.sendOK();
		}
	}
	else if(state == 2){
		if(npc.getSelected() == YES){
			if(player.getLevel() > 6){
				if(player.getMesos() > 149){
					player.giveMesos(-150);
					npc.addText("Awesome! #e150 mesos#n accepted! Alright, off to Victoria Island!");
					npc.sendNext();
				}
				else{
					npc.addText("What? You're telling me you wanted to go without any money? You're one weirdo...");
					npc.setSelected(NO);
					npc.sendOK();
				}
			}
			else{
				npc.addText("Let's see... I don't think you are strong enough. You'll have to be at least #bLevel 7#k ");
				npc.addText("to go to Victoria Island.");
				npc.setSelected(NO);
				npc.sendOK();
			}
		}
		else{
			npc.end();
		}
	}
	else if(state == 3){
		if(npc.getSelected() == YES){
			player.changeMap(104000000);
		}
		npc.end();
	}
}