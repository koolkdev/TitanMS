void npc_main(NPC @npc){
	//TODO
	Player@ player = npc.getPlayer();
	Map@ map = player.getChannel().getMap(910010000);
	map.setSpawn(false);
	map.clear();
	player.changeMap(910010000);
	npc.end();
}
