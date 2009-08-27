void portal_main(Player@ player, Portal@ portal){
    player.setGlobalVariable("fm_map", player.getMap().getID());
    player.setGlobalVariable("fm_portal", portal.getID());
    player.changeMap(910000000, "out00");
}