/* Made by ShingingDB */
void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("This is the town called #bAmherst#k, located at the northeast part of the Maple Island. You know that Maple Island is for beginners, right? I'm glad there are only weak monsters around this place.");
		npc.sendNext();
	}
	if(state == 1){
		npc.addText("If you want to get stronger, then go to #bSouthperry#k where there's a harbor. Ride on the gigantic ship and head to the place called #bVictoria Island#k. It's incomparable in size compared to this tiny island.");
		npc.sendBackNext();
	}
	if(state == 2){
		npc.addText("At the Victoria Island, you can choose your job. Is it called #bPerion#k...? I head there's a bare, desolate town where warriors live. A highland... what kind of a place would that be?");
		npc.sendBackOK();
		npc.end();
	}

}