void npc_main(NPC@ npc){
    Player@ player = npc.getPlayer();
    int state = npc.getState();
    if(state == 1 && npc.getSelected() == NO){
        npc.addText("You must have some business to take care of here, right?");
        npc.sendNext();
        npc.end();
        return;
    }
    else if(!player.getMap().getShip()){
        npc.addText("We will begin boarding 5 minutes before the takeoff. Please be patient and wait for a few minutes. Be aware that the ship will take off right on time, and we stop receiving tickets 1 minute before that, so please make sure to be here on time.");
        npc.sendNext();
        npc.end();
        return;
    }
    else if(getMinute()%10 == 9){
        npc.addText("The ship is getting ready for takeoff. I'm sorry, but you'll have to get on the next ride. The ride schedule is available through the usher at the ticketing booth.");
        npc.sendNext();
        npc.end();
        return;
    }
    if(state == 0){
        npc.addText("It looks like there's plenty of room for this ride. Please have your ticket ready so I can let you in. The ride will be long, but you'll get to your destination just fine. What do you think? Do you want to get on this ride?");
        npc.sendYesNo();
    }
    else if(state == 1){
        if(npc.getSelected() == YES){
            if(player.giveItem(4031044, -1) || player.giveItem(4031045, -1)){
                player.changeMap(220000111);
            }
            else{
                npc.addText("Oh no ... I don't think you have the ticket with you. I can't let you in without it. Please buy the ticket at the ticketing booth.");
                npc.sendNext();
            }
        }
        npc.end();
    }
}