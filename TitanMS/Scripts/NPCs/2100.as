void npc_main(NPC @npc){
	int state = npc.getState();
	int map = npc.getPlayer().getMap().getID();
	if(map == 0){
		if(state == 0){
			npc.addText("Welcome to the world of MapleStory. The purpose of this training camp is to ");
			npc.addText("help beginners. Would you like to enter this training camp? Some people start thier journey ");
			npc.addText(" without taking the training program. But I strongly recommend you take the training program first.");
			npc.sendYesNo();
		}
		else if(state == 1){
			if(npc.getSelected() == YES){
				npc.addText("Ok then, I will let you enter the training camp. Please follow your instructor's lead.");
				npc.sendNext();
			}
			else {
				npc.addText("Do you really wanted to start your journey right away?");
				npc.setState(npc.getState()+1);
				npc.sendYesNo();
			}
		}
		else if(state == 2){
			npc.getPlayer().changeMap(1);
			npc.end();
		}
		else if(state == 3){
			if(npc.getSelected() == NO){
				npc.addText("Please talk to me again when you finally made your decision.");
				npc.sendNext();
				npc.end();
			}
			else{
				npc.addText("It seems like you want to start your journey without taking the ");
				npc.addText("training program. Then, I will let you move on the training ground. Be careful~");
				npc.sendNext();
			}
		}
		else if(state == 4){
			npc.getPlayer().changeMap(40000);
			npc.end();
		}
	}
	else if (map == 1){
		if(state == 0){
			npc.addText("The is the image room where your first training program begins. ");
			npc.addText("In this room, you will have an advance look into the job of your choice. ");
			npc.sendNext();
		}
		else if(state == 1){
			npc.addText("Once you train hard enough, you will be entitled to occupy a job. ");
			npc.addText("You can become a Bowman in Henesys, a Magician in Ellinia, a Warrior in Perion, and a Thief in Kerening City..");
			npc.sendBackOK();
		}
		else if(state == 2){
			npc.end();
		}
	}
	else{
		npc.end();
	}
}