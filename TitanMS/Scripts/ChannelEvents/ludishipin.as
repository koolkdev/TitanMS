void event_main(Channel@ channel){
	channel.getMap(220000110).setShip(true); // Ludi Station
	channel.getMap(200000121).setShip(true); // Orbis Station
	channel.getMap(220000111).clear(); // Ludi Ship
	channel.getMap(200000122).clear(); // Orbis Ship
	channel.getMap(200090100).getPlayers().changeMap(200000100); // To orbis
	channel.getMap(200090110).getPlayers().changeMap(220000100); //  To ludi
}