void npc_main(NPC@ npc){
    int state = npc.getState();
    Player@ player = npc.getPlayer();
    int map = player.getMap().getID();
    if(map == 103000805){
        if(state == 0){
            npc.addText("Did you hunt a lot at the bonus map? Once you leave this place, you won't be able to come back and hunt again. Are you sure you want to leave here?");
            npc.sendYesNo();
        }
        else if(state == 1){
            if(npc.getSelected() == YES){
                player.changeMap(103000890);
            }
            else{
                npc.addText("I see. This map is designated for hunting, so it'll be best for you to hunt as much as possible before time runs out. If you feel like leaving this stage, by all means talk to me.");
                npc.sendOK();
            }
            npc.end();
        }
    }
    else if(map == 103000890){
        player.deleteVariable("kpq_1s_q");
        player.deleteVariable("kpq_1s_pdone");
        if(player.getItemAmount(4001007) > 0){
            player.giveItem(4001007, -player.getItemAmount(4001007));
        }
        if(player.getItemAmount(4001008) > 0){
            player.giveItem(4001008, -player.getItemAmount(4001008));
        }
        player.changeMap(103000000);
        npc.end();
    }
    else{
        if(state == 0){
            npc.addText("Once you leave the map, you'll have to restart the whole quest if you want to try it agian. Do you still want to leave this map");
            npc.sendYesNo();
        }
        else if(state == 1){
            if(npc.getSelected() == YES){
                player.changeMap(103000890);
            }
            else{
                npc.addText("I see. Teamwork is very important here. Please work harder with your fellow party members.");
                npc.sendOK();
            }
            npc.end();
        }
    }
}