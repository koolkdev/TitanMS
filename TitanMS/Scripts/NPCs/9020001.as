const int EXP_RATE = 10;

const int QUESTIONS = 9;
const int ROPES = 4;
const int PLATFORMS = 5;
const int BARRELS = 6;
const int REWARDS = 42;

const int [] q_ans = {8, 10, 10, 10, 20, 25, 25, 35, 15};
const string [] q_text = {"the number of levels needed to make the first job advancement as the wizard.",
    "the number of levels needed to make the first job advancement as the bowman.",
    "the number of levels needed to make the first job advancement as the thief.",
    "the number of levels needed to make the first job advancement as the warrior.",
    "the minimum amount of INT level needed to make the first job advancement as the wizard.",
    "the minimum amount of DEX level needed to make the first job advancement as the bowman.",
    "the minimum amount of DEX level needed to make the first job advancement as the thief.",
    "the minimum amount of STR level needed to make the first job advancement as the warrior.",
    "the experience points needed to advance from #rlevel 1 to level 2.#k"};

const int16 [] ropes_fh = {-4, -5, -6, -7};
const int16 [] platforms_fh_min = {14, 2, 8, 34, 25};
const int16 [] platforms_fh_max = {20, 6, 12, 38, 32};
const int16 [] barrels_fh = {117, 115, 113, 116, 114, 112};

const int [] rewards = {
    2000000, //100 Red Potions
    2000001, //100 Orange Potions
    2000002, //70 White Potions
    2000003, //100 Blue Potions
    2000004, //5 Elixers
    2000006, //50 Mana Elixirs
    2022000, //15 Pure Waters
    2022003, //15 Unagis
    2040505, //10% Overall DEF
    2040402, //10% Topwear DEF
    2040602, //10% Bottomwear DEF
    2040802, //10% Glove DEX
    2040002, //10% Helmet DEF
    2040502, //10% Overall DEX
    4010000, //8 Bronze Ores
    4010001, //8 Steel Ores
    4010002, //8 Mithril Ores
    4010003, //8 Adamantium Ores
    4010004, //8 Silver Ores
    4010005, //8 Orihalcon Ores
    4010006, //5 Gold Ores
    4020000, //8 Garnet Ores
    4020001, //8 Amethyst Ores
    4020002, //8 Aquamarine Ores
    4020003, //8 Emerald Ores
    4020004, //8 Opal Ores
    4020005, //8 Sapphire Ores
    4020006, //8 Topaz Ores
    4020007, //3 Diamond Ores
    4020008, //3 Black Crystal Ores
    1032000, //Weight Earrings ???
    1032002, //Sapphire Earrings
    1032004, //Gold Ring Earrings
    1032005, //Red Cross Earrings
    1032006, //Lightning Earrings
    1032007, //Emerald Earrings
    1032009, //Yellow Square Earrings
    1032010, //Star Earrings
    1002089, //Green Bamboo Hat
    1002090, //Blue Bamboo Hat
    1002026, //Brown Bamboo Hat
    4003000 //30 Screws
};
const int [] amounts = {100, 100, 70, 100, 5, 50, 15, 15, 1, 1, 1, 1, 1, 1, 8, 8, 8, 8, 8, 8, 5, 8, 8, 8, 8, 8, 8, 8, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 30};

void finishStage(Map@ map, bool portal){
    map.showEffect("quest/party/clear");
    map.playSound("Party1/Clear");
    map.showObject("gate");
    if(portal) map.enablePortal("next00");
}

void wrong(Map@ map){
    map.showEffect("quest/party/wrong_kor");
    map.playSound("Party1/Failed");
}

void giveExp(Players@ players, int exp){
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getHP() > 0)
            players[i].giveExp(exp*10);
}

void cleared(NPC@ npc){
    npc.addText("You all have cleared the quest for this stage. Use the portal to move to the next stage...");
    npc.sendNext();
    npc.end();
}

int getAmountInMap(Players@ players, Map@ map){
    int count = 0;
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getMap().getID() == map.getID() && players[i].getChannel().getID() == map.getChannel().getID())
            count++;
    return count;
}

int getPlayersOnFoothold(Players@ players, int16 fh){
    int count = 0;
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getFoothold() == fh)
            count++;
    return count;
}
int getPlayersOnFoothold(Players@ players, int16 min, int16 max){
    int count = 0;
    for(int i=0; i<players.getAmount(); i++)
        if(players[i].getFoothold() >= min && players[i].getFoothold() <= max)
            count++;
    return count;
}

void npc_main(NPC@ npc){
    int state = npc.getState();
    Player@ player = npc.getPlayer();
    Party@ party = player.getParty();
    Event@ event = player.getMap().getEvent();
    if(@party == null || @event == null){
        npc.end();
        return;
    }
    int inpq=0,inmap=0;
    if(party.getLeader() == player.getID()){
        inpq = getAmountInMap(party.getMembers(), player.getMap());
        inmap = event.getPlayers().getAmount();
    }
    int map = player.getMap().getID();
    if(map == 103000800){
        if(state == 0){
            if(player.getID() == party.getLeader()){
                if(party.getVariable("kpq_1s_first") == -1){
                    npc.addText("Hello. Welcome to the first stage. Look around and you'll see Ligators wandering around. When you defeat it, it'll cough up a piece of #bcoupon#k. Every member of the party other than the leader should talk to me, get a question, and gather up the same number of #bcoupons#k as the answer to the question I'll give to them.\r\nIf you gather up the right amount of coupons, I'll give the #bpass#k to that player. Once all the party members other than the leader gather up the #bpasses#k and give them to the leader, the leader will hand over the #bpasses#k to me, clearing the stage in the process. The faster ou take care of the stages, the more stages you'll be able to challenge, so I suggest you take care of things quickly and swiftly.\r\nWell then, best of luck to you.");
                    party.setVariable("kpq_1s_first", 1);
                    npc.end();
                }
                else if(party.getVariable("kpq_1s_done") == -1){
                    if(player.getItemAmount(4001008) >= inpq-1){
                        npc.addText("You gathered up #b" + (inpq-1) + " passes#k! Congratulation on clearing the stage! I'll make the portal that sends you to the next stage. There's a time limit on getting there, so please hurry. Best of luck to you all!");
                    }
                    else{
                        npc.addText("I'm sorry, but you are short on the number of passes. You need to give me the right number of passes; it should be the number of members of your party minus the leader, #b" + (inpq-1) + " passes#k to clear the stage. Tell your party members to solve the questions, gather up the passes, and give them to you.");
                        npc.end();
                    }
                }
                else{
                    cleared(npc);
                    return;
                }
            }
            else{
                if(player.getVariable("kpq_1s_pdone") == 1){
                    npc.addText("Wow you answered my question nicely. Here's the pass to the party; please hand it to the leader.");
                    npc.end();
                }
                else if(player.getVariable("kpq_1s_q") == -1){
                    npc.addText("Here, you need to collect #bcoupons#k by defeating the same number of ligators as the answer to the question asked individually.");
                    player.setVariable("kpq_1s_q", random(QUESTIONS));
                }
                else {
                    if(player.getItemAmount(4001007) == q_ans[player.getVariable("kpq_1s_q")]){
                        npc.addText("That's the right answer! For that you have just recevied a #bpass#k. Please hand it to your leader of the party.");
                        player.giveItem(4001007, -q_ans[player.getVariable("kpq_1s_q")]);
                        player.giveItem(4001008, 1);
                        player.setVariable("kpq_1s_pdone", 1);
                        npc.end();
                    }
                    else{
                        npc.addText("That's not the right answer. I can only give you the pass if you collect the same number of #bcoupons#k as the answer to the question suggests. I'll repeat the question.");
                    }
                }
            }
            npc.sendNext();
        }
        else if(state == 1){
            if(player.getID() == party.getLeader()){
                if(player.getItemAmount(4001008) >= inpq-1){
                    player.giveItem(4001008, -inpq+1);
                    party.setVariable("kpq_1s_done", 1);
                    finishStage(player.getMap(), true);
                    giveExp(player.getMap().getPlayers(), 100);
                }
                npc.end();
            }
            else{
                if(player.getVariable("kpq_1s_pdone") == -1 && player.getVariable("kpq_1s_q") != -1){
                    npc.addText("Here's the question. Collect the same number of coupons as " + q_text[player.getVariable("kpq_1s_q")]);
                    npc.sendBackNext();
                }
                else{
                    npc.end();
                }
            }
        }
        else if(state == 2){
            npc.end();
        }
    }
    else if(map == 103000801){
        if(party.getLeader() == player.getID()){
            if(party.getVariable("kpq_2s_first") == -1){
                    if(inpq != inmap){
                        npc.addText("It looks like not all the members of the party have gathered up yet. Everyone in your party needs to move from the previous stage to here in order to take on a task. Please have all your members ready here...");
                    }
                    else{
                        npc.addText("Hi. Welcome to the 2nd stage. Next to me, you'll see a number of ropes. Out of these ropes, #b3 are connected to the portal that sends you to the next stage#k. All you need to do is have #b3 party members to find the answer ropes and hang on them#k.\r\nBUT, it doesn't count as an answer if you hang on to the rope too low; please bring yourself up enough to be counted as a correct answer. Also, only 3 members of your party are allowed on the ropes. Once they are hanging on, the leader of the party must #bdouble-click me to check and see if the answer's correct or not#k. Now, find the right ropes to hang on!");
                        party.setVariable("kpq_2s_first", 1);
                        int [] ropes = {0, 0, 0, 0};
                        for(int i=0; i<3;){
                            int r = random(ROPES);
                            if(ropes[r] == 0){
                                ropes[r] = 1;
                                i++;
                            }
                        }
                        for(int i=0; i<ROPES; i++){
                            party.setVariable("kpq_2s_rope" + i, ropes[i]);
                        }
                    }
                    npc.sendNext();
            }
            else if(party.getVariable("kpq_2s_done") == 1){
                cleared(npc);
                return;
            }
            else{
                int [] ropes = {0, 0, 0, 0};
                for(int i=0; i<ROPES; i++){
                    ropes[i] = getPlayersOnFoothold(player.getMap().getPlayers(), ropes_fh[i]);
                }
                int onropes = 0;
                for(int i=0; i<ROPES; i++){
                    onropes += ropes[i];
                }
                if(onropes != 3){
                    npc.addText("It looks like you haven't found the 3 ropes just yet. Please think of a different combination of ropes. Only 3 are allowed to hang on to ropes, and if you hang on to too low, it won't count as an answer, so please keep that in mind. Keep going!");
                    npc.sendNext();
                }
                else{
                    bool ans = true;
                    for(int i=0; i<ROPES; i++){
                        if(ropes[i] != party.getVariable("kpq_2s_rope" + i)){
                            ans = false;
                            break;
                        }
                    }
                    if(ans){
                        finishStage(player.getMap(), true);
                        giveExp(player.getMap().getPlayers(), 200);
                        party.setVariable("kpq_2s_done", 1);
                    }
                    else{
                        wrong(player.getMap());
                    }
                }
            }
        }
        else{
            npc.addText("Let me describe the 2nd stage. You'll see a number of ropes next to me. Of these ropes, #b3 are connected to the portal that heads to the next stage#k. All you need to do is have #b3 party members to find the answer ropes and hang onto them#k.\r\nBUT, it doesn't count as an answer if you hang on to the rope too low; please bring yourself up enough to be counted as a correct answer. Also, only 3 members of you rparty are allowed on the ropes. Once they are hanging on, the leader of the party must #bdouble-click me to check and see if the answer's correct or not#k. Well then, best of luck to you!");
            npc.sendNext();
            npc.end();
        }
        npc.end();
    }
    else if(map == 103000802){
        if(party.getLeader() == player.getID()){
            if(party.getVariable("kpq_3s_first") == -1){
                    if(inpq != inmap){
                        npc.addText("It looks like not all the members of the party have gathered up yet. Everyone in your party needs to move from the previous stage to here in order to take on a task. Please have all your members ready here...");
                    }
                    else{
                        npc.addText("Hello. Welcome to the 3rd stage. Next to you you'll see barrels with kittens inside on top of the platforms. Out of these platform, #b3 of them lead to the portals for the next stage. 3 of the party members need to find the correct platform to step on and clear the stage.\r\nBUT, you need to stand firm right at the center of it, not standing on the edge, in order to be counted as a correct answer, so make sure to remember that. Also, only 3 members of your party are allowed on the platforms. Once the members are on them, the leader of the party must double-click on me to check and see if the answer's correct or not#k. Now, find the correct platforms~!");
                        party.setVariable("kpq_3s_first", 1);
                        int [] platforms = {0, 0, 0, 0, 0};
                        for(int i=0; i<3;){
                            int r = random(PLATFORMS);
                            if(platforms[r] == 0){
                                platforms[r] = 1;
                                i++;
                            }
                        }
                        for(int i=0; i<PLATFORMS; i++){
                            party.setVariable("kpq_3s_platform" + i, platforms[i]);
                        }
                    }
                    npc.sendNext();
            }
            else if(party.getVariable("kpq_3s_done") == 1){
                cleared(npc);
                return;
            }
            else{
                int [] platforms = {0, 0, 0, 0, 0};
                for(int i=0; i<PLATFORMS; i++){
                    platforms[i] = getPlayersOnFoothold(player.getMap().getPlayers(), platforms_fh_min[i], platforms_fh_max[i]);
                }
                int onplatforms = 0;
                for(int i=0; i<PLATFORMS; i++){
                    onplatforms += platforms[i];
                }
                if(onplatforms != 3){
                    npc.addText("It looks like you haven't found the 3 platforms just yet. Please think of a different combination of platforms. And please remember that only 3 can be on the platforms, stading firmly on the center of it, to be counted as answers. Keep going!");
                    npc.sendNext();
                }
                else{
                    bool ans = true;
                    for(int i=0; i<PLATFORMS; i++){
                        if(platforms[i] != party.getVariable("kpq_3s_platform" + i)){
                            ans = false;
                            break;
                        }
                    }
                    if(ans){
                        finishStage(player.getMap(), true);
                        giveExp(player.getMap().getPlayers(), 400);
                        party.setVariable("kpq_3s_done", 1);
                    }
                    else{
                        wrong(player.getMap());
                    }
                }
            }
        }
        else{
            npc.addText("Let me describe the 3rd stage. You'll see a bunch of barrels with kittens inside on the top of the platforms. #b3 of these platforms are connected to the portal that sends you to the next stage. 3 of the party members need to fin dth ecorrect platform to step on and clear the stage.\r\nBUT, you need to stand firm right at the center of it, not standing on the edge, in order to be counted as a correct answer, so make sure to remember that. Also, only 3 members of your party are allowed on the platforms. Once the members are on them, the leader of the party must double-click me to check and see if the answer's right or not#k. Well, then, best of luck to you~!");
            npc.sendNext();
        }
        npc.end();
    }
    else if(map == 103000803){
        if(party.getLeader() == player.getID()){
            if(party.getVariable("kpq_4s_first") == -1){
                    if(inpq != inmap){
                        npc.addText("It looks like not all the members of the party have gathered up yet. Everyone in your party needs to move from the previous stage to here in order to take on a task. Please have all your members ready here...");
                    }
                    else{
                        npc.addText("Hi. welcome to the 4th stage. Next to you you'll find a number of barrels. Out of these barrels 3 are connected to the portals to the next stage. #b3 members of the party need to find the right ones and stand on top of them#k to clear the stage. BUT please make sure to stand firm at the center of the barrel, not barely hanging on the edge, because that's the only way the answer counts. Also, only 3 members of your party are allowed on top of the barrels. Once the members are on top, the leader of the party must #bdouble-click me to check and see if the answer's right or not#k. Now, find the correct barrels~!");
                        party.setVariable("kpq_4s_first", 1);
                        int [] barrels = {0, 0, 0, 0, 0, 0};
                        for(int i=0; i<3;){
                            int r = random(BARRELS);
                            if(barrels[r] == 0){
                                barrels[r] = 1;
                                i++;
                            }
                        }
                        for(int i=0; i<BARRELS; i++){
                            party.setVariable("kpq_4s_barrel" + i, barrels[i]);
                        }
                    }
                    npc.sendNext();
            }
            else if(party.getVariable("kpq_4s_done") == 1){
                cleared(npc);
                return;
            }
            else{
                int [] barrels = {0, 0, 0, 0, 0, 0};
                for(int i=0; i<BARRELS; i++){
                    barrels[i] = getPlayersOnFoothold(player.getMap().getPlayers(), barrels_fh[i]);
                }
                int onbarrels = 0;
                for(int i=0; i<BARRELS; i++){
                    onbarrels += barrels[i];
                }
                if(onbarrels != 3){
                    npc.addText("It looks like you haven't found the 3 barrels just yet. Please think of a different combination of barrels. And please make sure to remember that only 3 can be on the top of the barrels, stading firm on the center of them, in order to be counted as the right answer. Keep going!");
                    npc.sendNext();
                }
                else{
                    bool ans = true;
                    for(int i=0; i<BARRELS; i++){
                        if(barrels[i] != party.getVariable("kpq_4s_barrel" + i)){
                            ans = false;
                            break;
                        }
                    }
                    if(ans){
                        finishStage(player.getMap(), true);
                        giveExp(player.getMap().getPlayers(), 800);
                        party.setVariable("kpq_4s_done", 1);
                    }
                    else{
                        wrong(player.getMap());
                    }
                }
            }
        }
        else{
            npc.addText("Let me describe the 4th stage. You'll see a bunch of barrels next to you. Out of these, #b3 are connected to the portals that take you to the next stage. 3 members of the party need to find the right ones and stand on top of them#k to clear the stage. BUT please make sure to stand firm at the center of the barrel, not barely hanging on the edge, because that's the only way the answer counts. Also, only 3 members of your party are allowed on top of the barrels. Once the members are on top, the leader of the party must #bdouble-click me to check and see if the answer's right or not.#k Well then, best of luck to you!");
            npc.sendNext();
        }
        npc.end();
    }
    else if(map == 103000804){
        if(state == 0){
            if(party.getVariable("kpq_5s_done") == 1){
                npc.addText("Incredible! You cleared all the stages to get to this point. Here's a small prize for your job well done. Before you accept it, however, please make sure your use and etc. inventories have empty slots available.");
                npc.sendNext();
            }
            else if(player.getID() == party.getLeader()){
                if(player.getItemAmount(4001008) >= 10){
                    npc.addText("Here's the portal that leads you to the last, bonus stage. It's a stage that allows you to defeat regular monsters a little easier. You'll be given a set amount of time to hunt as much as possible, but you can always leave the stage in the middle of it through the NPC. Again, congratulations on clearing all the stages. Take care...");
                    player.giveItem(4001008, -10);
                    finishStage(player.getMap(), false);
                    giveExp(player.getMap().getPlayers(), 1500);
                    party.setVariable("kpq_5s_done", 1);
                }
                else{
                    npc.addText("Hello. Welcome to the 5th and final stage. Walk around the map and you'll be able to find some Boss monsters. Defeat all of  them, gather up #b10 passes#k, and pleast get them to me. Once you earn your pass, the leader of your party will collect them, and then get them to me once the 10 passes are gathered up. The monsters may be familiar to you, but they may be much stronger than you may thing, so please be careful. Good luck!");
                }
                npc.sendNext();
                npc.end();
            }
            else{
                npc.end();
            }
        }
        else if(state == 1){
            if(party.getVariable("kpq_5s_done") == 1){
                if(player.checkSlot(EQUIP) && player.checkSlot(USE) && player.checkSlot(ETC)){
                    int reward = random(REWARDS);
                    player.giveItem(rewards[reward], amounts[reward]);
                    player.changeMap(103000805);
                    npc.end();
                }
                else{
                    npc.addText("Your use and etc. inventory need to have at least one empty slot available to receive your rewards. Please make the necessary adjustments and then talk to me.");
                    npc.sendBackNext();
                }
            }
            else{
                npc.end();
            }
        }
        else{
            npc.end();
        }
    }
}