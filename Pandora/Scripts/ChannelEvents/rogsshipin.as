void spawnBalrogs(Map@ map){
    map.setShip(true);
    map.spawnMob(9500171, map.getShipPosition().x, map.getShipPosition().y);
    map.spawnMob(9500171, map.getShipPosition().x, map.getShipPosition().y);
}

void event_main(Channel@ channel){
    if(random(3) == 0){ // Ellinia Ship
        spawnBalrogs(channel.getMap(200090000));
    }
    if(random(3) == 0){ // Orbis Ship
        spawnBalrogs(channel.getMap(200090010));
    }
}