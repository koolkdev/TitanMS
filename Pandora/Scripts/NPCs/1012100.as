/* Made by monsoon2004 */
void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		if(player.getJob() == 0){
			if(player.getLevel() >= 10 && player.getDex() >= 25){
				npc.addText("Would you like to become an Archer?");
				npc.sendYesNo();
			}
			else{
				npc.addText("I'm sorry but you're too weak at the moment.");
				npc.sendOK();
				npc.end();
			}
		}
		else if(player.getJob() == 300 && player.getLevel() >= 30){
			if(player.getItemAmount(4031010) == 0){
				if(player.getItemAmount(4031012) > 0){
					npc.addText("I knew the test wouldn't be a problem for you. Are you ready to choose your next path?");
					npc.sendYesNo();
				}
				else{
					npc.addText("You have improved significantly. Would you like to take the second job test?");
					npc.sendYesNo();
				}
			}
			else{
				npc.addText("Please come back after you have passed the test.");
				npc.sendOK();
				npc.end();
			}
		}
		else{
			npc.addText("How goes your training?");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			if(player.getJob() == 0){
				player.setJob(300);
				npc.addText("You are now an Archer.");
				player.setSP(player.getSP() + 1);
				npc.sendOK();
				npc.end();
			}
			else if(player.getJob() == 300){
				if(player.getItemAmount(4031010) == 0){
					if(player.getItemAmount(4031012) == 1){
						npc.addText("Choose your path. \r\n#L0#Hunter#l\r\n#L1#Crossbowman#l");
						npc.sendSimple();
					}
					else{
						npc.addText("Very well, I will give you my recommendation letter. Take it to my assistant and she will give you your next task.");
						npc.sendOK();
						npc.end();
						player.giveItem(4031010, 1);
					}
				}
			}
		}
		else{
			npc.addText("Hmm.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 2){
		int type = npc.getSelected();
		npc.setVariable("type", type);
		if(type == 0){
			player.setJob(310);
			npc.addText("You are now a Hunter, nothing escapes your bow.");
		}
		else if(type == 1){
			player.giveItem(4031012, -1);
			player.setJob(320);
			npc.addText("You are now a Sniper, extremely accurate and deadly.");
		}
		player.giveItem(4031012, -1);
		player.setSP(player.getSP() + 1);
		npc.sendOK();
		npc.end();
	}
}