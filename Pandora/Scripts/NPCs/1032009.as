void npc_main(NPC@ npc){
    Player@ player = npc.getPlayer();
    int state = npc.getState();
    if(state == 0){
        npc.addText("Do you want to leave the waiting room? You can, but the ticket is NOT refundable. Are you sure you still want to leave this room?");
        npc.sendYesNo();
    }
    else if(state == 1){
        if(npc.getSelected() == YES){
            player.changeMap(101000300);
        }
        else{
            npc.addText("You'll get to your destination in a few. Go ahead and talk to other people, and before you know it, you'll be there already.");
            npc.sendNext();
        }
        npc.end();
    }
}