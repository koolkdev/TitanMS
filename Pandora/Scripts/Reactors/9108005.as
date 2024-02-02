void reactor_main(Reactor@ reactor){
	Reactor@ r = reactor.getMap().getReactorByReactorID(9101000);
	if(!isNULL(r))
		r.setState(r.getState()+1);
}