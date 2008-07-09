/* Made by monsoon2004 */
void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		if(npc.getItemAmount(4031013) >= 30){
				npc.addText("You are stronger than I thought. I'll give you a medal to show Athena that you have accomplished this feat.");
				npc.sendNext();
		}
		else{
			npc.addText("You aren't quite finished yet.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 1){
		npc.giveItem(4031013, -npc.getItemAmount(4031013));
		npc.giveItem(4031012, 1);
		npc.teleport(106010000);
		npc.end();
	}
}