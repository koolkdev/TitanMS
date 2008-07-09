void npc_main(NPC @npc){
	//TODO
	Player@ player = npc.getPlayer();
	Map@ map = player.getChannel().getMap(910010000);
	map.setSpawn(false);
	map.killMobs();
	map.respawnReactors();
	map.clearDrops();
	player.changeMap(910010000);
	npc.end();
}
