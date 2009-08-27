const int OPTIONS = 3;

const int [] maps = {200000110, 200000120, 200000140};
const string [] names = {"Ellinia", "Ludibrium", "Mu Lung"};
const string [] ways = {"the ship", "the ship", "Hak"};
    
void npc_main(NPC@ npc){
    int state = npc.getState();
    if(state == 0){
        npc.addText("Orbis Station has lots of platforms available to choose from. You need to choose the one that'll take you to the destination of your choice. Which platform will you take?\r\n");
        for(int i=0; i<OPTIONS; i++){
            npc.addText("#b#L" + i + "#The platform to "+ ways[i] + " that heads to " + names[i] + "#l#k\r\n");
        }
        npc.sendSimple();
    }
    else if(state == 1){
        int dest = npc.getSelected();
        npc.setVariable("dest", dest);
        npc.addText("Even if you look the wrong passage you can get back here using the portal, so no worries. Will you move to the #bplatform to "+ ways[dest] + " that heads to " + names[dest] + "#k?");
        npc.sendYesNo();
    }
    else if(state == 2){
        if(npc.getSelected() == YES){
            npc.getPlayer().changeMap(maps[npc.getVariable("dest")], "west00");
        }
        else{
            npc.addText("Please make sure you know where you are going and then go to the platform through me. The ride is on schedule so you better not miss it!");
            npc.sendNext();
        }
        npc.end();
    }
}