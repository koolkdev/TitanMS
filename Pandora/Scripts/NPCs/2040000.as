void npc_main(NPC@ npc){
    Player@ player = npc.getPlayer();
    int price = (player.getJob() == 0) ? 1200 : 6000;
        int state = npc.getState();
    if(state == 0){
        npc.addText("Hello, I'm in charge of selling tickets for the ship ride to Orbis Station of Ossyria. The ride to Orbis takes off every 10 minutes, begging on the hour, and it'll cost you #b" + price + " mesos#k. Are you sure you want to purchase #b#t4031045##k?");
        npc.sendYesNo();
    }
    else if(state == 1){
        if(npc.getSelected() == YES){
            if(player.getMesos() >= price){
                if(player.giveItem(4031045, 1)){
                    player.giveMesos(-price);
                    npc.end();
                    return;
                }
            }
            npc.addText("Are you sure you have #b" + price + " mesos#k? If so, then I urge you to check your etc. inventory, and see if it's full or not.");
            npc.sendOK();
        }
        else{
            npc.addText("You must have some business to take care of here, right?");
            npc.sendNext();
        }
        npc.end();
    }
}