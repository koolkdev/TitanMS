const int OPTIONS = 2;

const int [] tickets = {4031047, 4031074};
const int [] times = {15, 10};
const string [] names = {"Ellinia of Victoria Island", "Ludibrium"};
const string [] names2 = {"Ellinia of Victoria Island", "Ludibrium"};

void npc_main(NPC@ npc){
    Player@ player = npc.getPlayer();
    int state = npc.getState();
    int [] prices = {5000, 6000};
    if(player.getJob() == 0){
        for(int i=0; i<OPTIONS; i++){
            prices[i] /= 5;
        }
    }
    if(state == 0){
        npc.addText("Hello, I'm in charge of selling tickets for the ship ride for every destination. Which ticket would you like to purchase?\r\n");
        for(int i=0; i<OPTIONS; i++){
            npc.addText("#b#L" + i + "#" + names[i] + "#l#k\r\n");
        }
        npc.sendSimple();
    }
    else if(state == 1){
        int dest = npc.getSelected();
        npc.setVariable("dest", dest);
        npc.addText("The ride to " + names2[dest] + " takes off every " + times[dest] + " minutes, beginning on the hour, and it'll cost you #b" + prices[dest] + " mesos#k. Are you sure you want to purchase #b#t" + tickets[dest] + "##k?");
        npc.sendYesNo();
    }
    else if(state == 2){
        int dest = npc.getVariable("dest");
        if(npc.getSelected() == YES){
            if(player.getMesos() >= prices[dest]){
                if(player.giveItem(tickets[dest], 1)){
                    player.giveMesos(-prices[dest]);
                    npc.end();
                    return;
                }
            }
            npc.addText("Are you sure you have #b" + prices[dest] + " mesos#k? If so, then I urge you to check your etc. inventory, and see if it's full or not.");
            npc.sendOK();
        }
        else{
            npc.addText("You must have some business to take care of here, right?");
            npc.sendNext();
        }
        npc.end();
    }
}