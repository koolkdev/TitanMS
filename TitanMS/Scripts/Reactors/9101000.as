void reactor_main(Reactor@ reactor){
	Position pos = reactor.getPosition();
	reactor.getMap().spawnMob(9300061, pos.x, pos.y);
	reactor.getMap().setSpawn(true);
}