void npc_main(NPC@ npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("Hello! the evil yellow snails are coming!");
		npc.sendNext();
	}
	else{
		Position pos = npc.getPlayer().getPosition();
		Map@ map = npc.getPlayer().getMap();
		for(int i=0; i<10; i++){
			map.spawnMob(130101, pos.x, pos.y);
		}
		npc.end();
	}
}