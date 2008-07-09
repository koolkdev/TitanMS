void npc_main(NPC @npc){
	int state = npc.getState();
	Player@ player = npc.getPlayer();
	if(state == 0){
		npc.addText("Have you heard of the beach with a spectacular view of the ocean called #bFlorina Beach#k, located near Lith Harbor? I can take you there right now for either #b1500 mesos#k, or if you have a #bVIP Ticket to Florina Beach#k with you, in which case you'll be in for free. #b\r\n#L0#I'll pay 1500 mesos.#l\r\n#L1#I have a VIP Ticket to Florina Beach.\r\n#L2#What is a VIP Ticket to Florina Beach?#l#k");
		npc.sendSimple();
	}
	else if(state == 1){
		int type = npc.getSelected();
		npc.setVariable("type", type);
		if(type == 0){
			npc.addText("So you want to pay #b1500 mesos#k and leave for Florina Beach? Alright then, but just be aware that you may be running into some monsters around there too. Okay, would you like to head over to Florina Beach right now?");
			npc.sendYesNo();
		}
		if(type == 1){
			npc.addText("So you have a #bVIP Ticket to Florina Beach#k? You can always head over to Florina Beach with that. Alright then, but just be aware that you may be running into some monsters there too. Okay, would you like to head over to Florina Beach right now?");
			npc.sendYesNo();
		}
		if(type == 2){
			npc.addText("You must be curious about the #bVIP Ticket to Florina Beach#k. Haha, that's very understandable. VIP Ticket to Florina Beach is an item where as long as you have in possession, you may make your way to Florina Beach for free. It's such a rare item that even we had to buy those, but unfortunately I lost mine a few weeks ago during my precious summer break.");
			npc.sendNext();
		}
	}
	else if(state == 2){
		int type = npc.getVariable("type");
		if(type == 0){
			if(npc.getSelected() == YES){
				if(player.getMesos() > 1499){	
					player.giveMesos(-1500);	
					player.changeMap(110000000);
					npc.end();
				}
				else{
					npc.addText("I think you're lacking mesos. There are many ways to gather up some money, you know, like... selling your armor... defeating monsters... doing quests... you know what I'm talking about.");
            					npc.sendNext();
					npc.end();
				}
			}
			else{
				npc.addText("You must have some business to take care of here. You must be tired from all that traveling and hunting. Go take some rest, and if you feel like changing your mind, then come talk to me.");
				npc.sendNext();
				npc.end();
			}
		}
		else if(type == 1){
			if(npc.getSelected() == YES){
				if(player.getItemAmount(4031134) >= 1){
					player.changeMap(110000000);
					npc.end();
				}
				else{
					npc.addText("Hmmm, so where exactly is your #bVIP Ticket to Florina Beach#k?? Are you sure you have them? Please double-check.");
					npc.sendOK();
					npc.end();
				}
			}
			else{
				npc.addText("You must have some business to take care of here. You must be tired from all that traveling and hunting. Go take some rest, and if you feel like changing your mind, then come talk to me.");
				npc.sendNext();
				npc.end();
			}
		}	
		else if(type == 2){
			npc.addText("I came back without it, and it just feels awful not having it. Hopefully someone picked it up and put it somewhere safe. Anyway this is my story and who knows, you may be able to pick it up and put it to good use. If you have any questsions, feel free to ask.");
			npc.sendBackNext();
			npc.end();
		}
	}
}