void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("Hey there, Pretty~ I am Roger who teachs you adroable new Maplers with lots of information.");
		npc.sendNext();
	}
	else if(state == 1){
		npc.addText("I know you are busy! Please spare me some time~ I can teach you some useful information! Ahahaha!");
		npc.sendBackNext();
	}
	else if(state == 2){
		npc.addText("So..... Let me just do this for fun! Abaracadabra~!");
		npc.sendAcceptDecline();
	}
	else if(state == 3){
		if(npc.getSelected() == ACCEPT){
			npc.setPlayerHP(25);
			npc.giveItem(2010007, 1);
			npc.addQuest(1021);
			npc.setState(npc.getState()+1);
			npc.addText("Surprised? If HP becomes 0, then you are in trouble. Now, I will give you #r#t2010007##k. Please take it. ");
			npc.addText("You will feel stronger. Open the Item window and double click to consume. Hey, It's very simple to open the Item window. Just press #bI#k on your keyboard.");
			npc.sendNext();
		}
		else{
			npc.addText("I can't believe you just have turned down a attractive guys like me!");
			npc.sendNext();
			npc.end();
		}
	}
	else if(state == 4){
		npc.addText("Surprised? If HP becomes 0, then you are in trouble. Now, I will give you #r#t2010007##k. Please take it. ");
		npc.addText("You will feel stronger. Open the Item window and double click to consume. Hey, It's very simple to open the Item window. Just press #bI#k on your keyboard.");
		npc.sendNext();
	}
	else if(state == 5){
			npc.addText("Please take all #t2010007#s that I gave you. You will be able to see the HP bar increasing. "); 
			npc.addText("Please talk to me again when you recover your HP 100%");
			npc.sendBackOK();
	}
	else if(state == 6){
		npc.end();
	}
}