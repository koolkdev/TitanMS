const int MEMBERS = 4;

int getAmountInMap(Players@ players, Map@ map){
    int count = 0;
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getMap().getID() == map.getID() && players[i].getChannel().getID() == map.getChannel().getID())
            count++;
    return count;
}

bool checkLevels(Players@ players, int minlevel, int maxlevel){
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getLevel() < minlevel || players[i].getLevel() > maxlevel)
            return false;
    return true;
}

void npc_main(NPC@ npc){
    int state = npc.getState();
    Player@ player = npc.getPlayer();
    if(state == 0){
        if(@player.getParty() == null || player.getParty().getLeader() != player.getID()){
            npc.addText("How about you and your party members collectively beating a quest? Here you'll find obstacles and problems where you won't be able to beat it unless with great teamwork. If you want to try it, please tell the #bleader of your party#k to talk to me.");
            npc.sendNext();
            npc.end();
        }
        else{
            Players@ members = player.getParty().getMembers();
            if(getAmountInMap(members, player.getMap()) != MEMBERS || members.getAmount() != MEMBERS){
                npc.addText("Your party is not a party of four. Please come back when you have four party members.");
                npc.sendNext();
            }
            else if(!checkLevels(members, 21, 30)){
                npc.addText("Someone in your your party does not have a level between 21 ~ 30. Please double-check.");
                npc.sendNext();
            }
            else if(!player.getChannel().createEvent("kereningpq", player.getParty())){
                npc.addText("Some other party has already gotten in to try clearing the quest. Please try again later.");
                npc.sendNext();
            }
            npc.end();
        }
    }
}