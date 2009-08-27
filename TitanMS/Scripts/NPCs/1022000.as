/* Made by monsoon2004 */
void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		if(player.getJob() == 000){
			if(player.getLevel() >= 10 && player.getStr() >= 35){
				npc.addText("Welcome. Have you come to become a Swordsman?");
				state = 1;
				npc.sendYesNo();
			}
			else{
				npc.addText("You're too weak. Come back when you're at least level 10 and have a minimum of 35 str.");
				npc.sendOK();
				npc.end();
			}
		}
		else if(player.getJob() == 100 && player.getLevel() >= 30){
			if(player.getItemAmount(4031008) == 0){
				if(player.getItemAmount(4031012)>0){
					npc.addText("I'm glad to see that you passed the test. Would you like to choose your next path now?");
					npc.sendYesNo();
				}
				else{
					npc.addText("I see that you're a lot stronger than you were when you first came here. Would you like to become even more powerful?");
					state = 1;
					npc.sendYesNo();
				}
			}
			else{
				npc.addText("There are no shortcuts, you must pass the test or I won't help you.");
				npc.sendOK();
				npc.end();
			}
		}
		else{
			npc.addText("Remember, strength is your friend.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			if(player.getJob() == 0){
				player.setJob(100);
				npc.addText("You now have some understanding of how to fight. It is up to you to train and increase your power.");
				player.setSP(player.getSP() + 1);
				npc.sendOK();
				npc.end();
			}
			else if(player.getJob() == 100){
				if(player.getItemAmount(4031008) == 0){
					if(player.getItemAmount(4031012) == 1){
						npc.addText("Choose your path. \r\n#L0#Fighter#l\r\n#L1#Page#l\r\n#L2#Spearman#l");
						npc.sendSimple();
					}
					else{
						npc.addText("If you're sure that you're ready, then take this letter to my assistant so that he may test you. Good luck young warrior.");
						npc.sendOK();
						npc.end();
						player.giveItem(4031008, 1);
					}
				}
			}
		}
		else{
			npc.addText("Very well.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 2){
		int type = npc.getSelected();
		npc.setVariable("type", type);
		if(type == 0){
			player.giveItem(4031012, -1);
			player.setJob(110);
			player.setSP(player.getSP() + 1);
			npc.addText("You are now a Fighter, with the ability to masterfully wield swords and axes no one would dare challenge you.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 1){
			player.giveItem(4031012, -1);
			player.setJob(120);
			player.setSP(player.getSP() + 1);
			npc.addText("You are now an Page, your range of elemental attacks can take almost any enemy by surprise.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 2){
			player.giveItem(4031012, -1);
			player.setJob(130);
			player.setSP(player.getSP() + 1);
			npc.addText("You are now a Spearman, easily capable of keeping any enemy at bay until you are ready to finish them off.");
			npc.sendOK();
			npc.end();
		}
	}
}