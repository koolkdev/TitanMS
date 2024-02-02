void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("You have finished all your trainings. Good job. ");
		npc.addText("You seem to be ready to start with the journey right away! Good , I will let you on to the next place.");
		npc.sendNext();
	}
	else if(state == 1){
		npc.addText("But remember, once you get out of here, you will enter a village full with monsters. Well them, good bye!");
		npc.sendBackNext();
	}
	else if(state == 2){
		npc.getPlayer().changeMap(40000);
		npc.end();

	}
}