void reactor_main(Reactor@ reactor, Player@ player){
	Position pos = reactor.getPosition();
	reactor.getMap().spawnMob(9300061, pos.x, pos.y);
	reactor.getMap().setSpawn(true);
}