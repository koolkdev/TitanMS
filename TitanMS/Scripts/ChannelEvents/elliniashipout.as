void event_main(Channel@ channel){
	channel.getMap(101000300).setShip(false); // Ellinia Station
	channel.getMap(200000111).setShip(false); // Orbis Station
	channel.getMap(101000301).getPlayers().changeMap(200090010); // To orbis
	channel.getMap(200000112).getPlayers().changeMap(200090000); //  To ellinia
}