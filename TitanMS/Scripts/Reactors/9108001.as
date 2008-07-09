void reactor_main(Reactor@ reactor, Player@ player){
	Reactor@ r = reactor.getMap().getReactorByReactorID(9101000);
	r.setState(r.getState()+1);
}