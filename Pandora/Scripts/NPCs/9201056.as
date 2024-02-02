void npc_main(NPC @npc){//9201056
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	int map = npc.getPlayer().getMap().getID();
	if( map == 600000000){
		if(state == 0){
			npc.addText("Hey, there. Want to take a trip deeper into the Masterian wilderness? A lot of this continent is still quite unknown and untamed... so there's still not much in the way of roads. Good thing we've got this baby... We can go offroading, and in style too!");
			npc.sendNext();
		}
		else if(state == 1){
			npc.addText("Right now, I can drive you to the #bPhantom Forest#k. The old #bPrendergast Mansion#k is located there. Some people say the place is haunted!");
			npc.sendBackNext();
		}
		else if(state == 2){
			npc.addText("What do you say...? Want to head over there?");
			npc.sendYesNo();
		}
		else if(state == 3){
			if(npc.getSelected() == YES){
				npc.addText("Alright! Buckle your seat belt, and let's head to the Mansion!\r\nIt's going to get bumpy!");
				npc.sendNext();
			}
			else{
				npc.addText("Really? I don't blame you... Sounds like a pretty scary place to me too! If you change your mind, I'll be right here.");
				npc.sendOK();
			}
		}
		else if(state == 4){
			if(npc.getSelected() == YES){
				npc.getPlayer().changeMap(682000000);
			}
			npc.end();
		}
	}
	if(map == 682000000){
		if(state == 0){
			npc.addText("Hey, there. Hope you had fun here! Ready to head back to #bNew Leaf City#k?");
			npc.sendYesNo();
		}
		else if(state == 1){
			if(npc.getSelected() == YES){
				npc.addText("Back to civilization it is! Hop in and get comfortable back there... We'll have you back to the city in a jiffy!");
				npc.sendNext();
			}
			else{
				npc.addText("Oh, you want to stay and look around some more? That's understandable. If you wish to go back to #bNew Leaf City#k, you know who to talk to!");
				npc.sendOK();
			}
		}
		else if(state == 2){
			if(npc.getSelected() == YES){
				npc.getPlayer().changeMap(600000000);
			}
			npc.end();
		}
	}
}

