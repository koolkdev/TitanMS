void npc_main(NPC@ npc){
    int state = npc.getState();
    Player@ player = npc.getPlayer();
    int map = player.getMap().getID();
    if(map == 200000141){
        if(player.getChannel().isEventActive("orbistomulung")){
            npc.addText("Someone else is on the way to Mu Lung right now. Talk to me in a little bit.");
            npc.sendNext();
            npc.end();
            return;
        }
        if(state == 0){
            npc.addText("Hello there? I'm the crane that flies from #bOrbis#k to #bMu Lung#k and back. I fly around all the time, so I figured, why not make some money by taking travelers like you along for a small fee? It's good business for me. Anyway, what do you think?\r\n");
            npc.addText("#b#L0#Mu Lung(6000 mesos)#l#k");
            npc.sendSimple();
        }
        else if(state == 1){
            if(player.getMesos() >= 6000){
                if(player.getChannel().createEvent("orbistomulung", player)){
                    player.giveMesos(-6000);
                }
                else{
                    npc.addText("Someone else is on the way to Mu Lung right now. Talk to me in a little bit.");
                    npc.sendNext();
                }
            }
            else{
                npc.addText("Are you sure you have enough mesos?");
                npc.sendNext();
            }
            npc.end();
        }
    }
    else if(map == 250000100){
        if(state == 0){
            npc.addText("Hello there? I'm the crane that flies from #bMu Lung#k to #bOrbis#k  and back. I fly around all the time, so I figured, why not make some money by taking travelers like you along for a small fee? It's good business for me. Anyway, what do you think?\r\n");
            npc.addText("#b#L0#Orbis(6000 mesos)#l#k\r\n");
            npc.addText("#b#L1#Herb Town(1500 mesos)#l#k");
            npc.sendSimple();
        }
        else if(state == 1){
            if(npc.getSelected() == 0){
                if(player.getChannel().isEventActive("mulungtoorbis")){
                    npc.addText("Someone else is on the way to Orbis right now. Talk to me in a little bit.");
                    npc.sendNext();
                    npc.end();
                    return;
                }
                if(player.getMesos() >= 6000){
                    if(player.getChannel().createEvent("mulungtoorbis", player)){
                        player.giveMesos(-6000);
                    }
                    else{
                        npc.addText("Someone else is on the way to Mu Lung right now. Talk to me in a little bit.");
                        npc.sendNext();
                    }
                }
                else{
                    npc.addText("Are you sure you have enough mesos?");
                    npc.sendNext();
                }
                npc.end();
            }
            else{
                npc.addText("Will you move to #bHerb Town#k now? If you have #b1500mesos#k, I'll take you there right now.");
                npc.sendYesNo();
            }
        }
        else if(state == 2){
            if(npc.getSelected() == YES){
                if(player.getMesos() >= 1500){
                    player.changeMap(251000000);
                    player.giveMesos(-1500);
                }
                else{
                    npc.addText("Are you sure you have enough mesos?");
                    npc.sendNext();
                }
            }
            else{
                npc.addText("OK. If you ever change your mind, please let me know.");
                npc.sendNext();
            }
            npc.end();
        }
    }
    else if(map == 251000000){
        if(state == 0){
            npc.addText("Hello there? I'm the crane that flies from #bOrbis#k to #bMu Lung#k and back. I fly around all the time, so I figured, why not make some money by taking travelers like you along for small fee? It's good business for me. Anyway, what do you think? Do you want to fly to #bMu Lung#k right now? I only charge #b1500 mesos#k.");
            npc.sendYesNo();
        }
        else if(state == 1){
            if(npc.getSelected() == YES){
                if(player.getMesos() >= 1500){
                    player.changeMap(250000100);
                    player.giveMesos(-1500);
                }
                else{
                    npc.addText("Are you sure you have enough mesos?");
                    npc.sendNext();
                }
            }
            else{
                npc.addText("OK. If you ever change your mind, please let me know.");
                npc.sendNext();
            }
            npc.end();
        }
    }
}