void event_main(Channel@ channel){
	channel.getMap(220000110).setShip(false); // Ludi Station
	channel.getMap(200000121).setShip(false); // Orbis Station
	channel.getMap(220000111).getPlayers().changeMap(200090100); // To orbis
	channel.getMap(200000122).getPlayers().changeMap(200090110); //  To ludi
}