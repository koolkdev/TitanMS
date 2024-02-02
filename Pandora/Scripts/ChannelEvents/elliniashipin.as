void event_main(Channel@ channel){
	channel.getMap(101000300).setShip(true); // Ellinia Station
	channel.getMap(200000111).setShip(true); // Orbis Station
	channel.getMap(101000301).clear(); // Ellinia Ship
	channel.getMap(200000112).clear(); // Orbis Ship
	channel.getMap(200090010).getPlayers().changeMap(200000100); // To orbis
	channel.getMap(200090011).getPlayers().changeMap(200000100); // To orbis
	channel.getMap(200090000).getPlayers().changeMap(101000300); //  To ellinia
	channel.getMap(200090001).getPlayers().changeMap(101000300); //  To ellinia
}