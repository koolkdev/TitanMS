void removeBalrogs(Map@ map){
    map.setShip(false);
    map.changeMusic("Bgm04/UponTheSky");
    map.clearMobs();
}

void event_main(Channel@ channel){
    Map@ shipe = channel.getMap(200090000);
    if(shipe.getShip()){ // Ellinia Ship
        removeBalrogs(shipe);
    }
    Map@ shipo = channel.getMap(200090010);
    if(shipo.getShip()){ // Orbis Ship
        removeBalrogs(shipo);
    }
}