void portal_main(Player@ player, Portal@ portal){
    int map = player.getGlobalVariable("fm_map");
    if(map == -1){
        player.changeMap(102000000, "market00");
    }
    else{
        int portal = player.getGlobalVariable("fm_portal");
        player.changeMap(map, portal);
        player.deleteGlobalVariable("fm_map");
        player.deleteGlobalVariable("fm_portal");
    }
}