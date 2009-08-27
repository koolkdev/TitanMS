const int[] bows = {1452002, 1452003, 1452001, 1452000, 1452005, 1452006, 1452007};
const int[] bows_mesos = {800, 2000, 3000, 5000, 30000, 40000, 80000};
void npc_main(NPC @npc){ // Vicious
    int get = sizeof(bows);
    Player@ player = npc.getPlayer();
	int state = npc.getState();
	int type = npc.getSelected();
	if(state == 0){
		npc.addText("Hey you~ Don't you need something? Well, nothing's free around here, but... with a few materials and a little service fee, I can make some items for you. So what do you think? Wanna make a deal? Just please make sure no one in this town knows about this.");
		npc.sendYesNo();
	}
	else if(state == 1){
		if(npc.getSelected() == YES){
			npc.addText("Nice~! What can I do for you? All you need to do is say it! ");
			npc.addText("#b\r\n#L0#Make a bow#l\r\n#L1#Make a crossbow#l\r\n#L2#Make a glove#l\r\n#L3#Upgrade a glove#l\r\n#L4#Create materials#l\r\n#L5#Create arrows#l#k");
			npc.sendSimple();
		}
		else{
			npc.addText("You don't seem too interested. If you need something later on, thought, feel free to come back. I can even hook you up with an item you can't find in stores.");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 2){
		if(type == 0){ // Make a bow
			npc.addText("I'll make you a bow if you get me some materials. What kind of a bow do you want? ");
			npc.addText("#b\r\n#L0#War Bow#k(Level limit : 10, Bowman)#l#b\r\n#L1#Composite Bow#k(Level limit : 15, Bowman)#l#b\r\n#L2#Hunter's Bow#k(Level limit : 20, Bowman)#l#b\r\n#L3#Battle Bow#k(Level limit : 25, Bowman)#l#b\r\n#L4#Ryden#k(Level limit : 30, Bowman)#l#b\r\n#L5#Red Viper#k(Level limit : 35, Bowman)#l#b\r\n#L6#Vaulter 2000#k(Level limit : 40, Bowman)#l");
			npc.sendSimple();
		}
		else if(type == 1){ // Make a crossbow
			npc.addText("I'll make you a crossbow if you get me some materials. What kind of a crossbow do you want?");
			npc.addText("#b\r\n#L7#Crossbow#k(Level limit : 12, Bowman)#l#b\r\n#L8#Battle Crossbow#k(Level limit : 18, Bowman)#l#b\r\n#L9#Balanche#k(Level limit : 22, Bowman)#l#b\r\n#L10#Mountain Crossbow#k(Level limit : 28, Bowman)#l#b\r\n#L11#Eagle Crow#k(Level limit : 32, Bowman)#l#b\r\n#L12#Heckler#k(Level limit : 38, Bowman)#l#b\r\n#L13#Silver Crow#k(Level limit : 42, Bowman)#l#b\r\n#L14#Rower#k(Level limit : 50, Bowman)#l");
			npc.sendSimple();
		}
		else if(type == 2){ // Make a glove
			npc.addText("I'll make you a glove if you get me some materials. What kind of glove do you want?");
			npc.addText("#b\r\n#L15#Basic Archer Gloves#k(Level limit : 15, Bowman)#l#b\r\n#L16#Blue Diros#k(Level limit : 20, Bowman)#l#b\r\n#L17#Blue Savata#k(Level limit : 25, Bowman)#l#b\r\n#L18#Brown Marker#k(Level limit : 30, Bowman)#l#b\r\n#L19#Bronze Scaler#k(Level limit : 35, Bowman)#l#b\r\n#L20#Aqua Brace#k(Level limit : 40, Bowman)#l#b\r\n#L21#Blue Willow#k(Level limit : 50, Bowman)#l#b\r\n#L22#Oaker Garner#k(Level limit : 60, Bowman)#l");
			npc.sendSimple();
		}
		else if(type == 3){ // Upgrade a glove
			npc.addText("Want to upgrade a glove? Better be careful with it though. All the items that will be used for upgrading will be gone, and if you use an item that has been #rupgraded#k with a scroll, the effect will disappear when upgraded. Think carefully.");
			npc.sendNext();
		}
		else if(type == 4){ // Create materials
			npc.addText("So you want some materials, huh? Alright... what kind? ");
			npc.addText("#b\r\n#L23#Make Processed Wood with Tree Branch#l\r\n#L24#Make Processed Wood with Firewood#l\r\n#L25#Make Screws#l#k");
			npc.sendSimple();
		}
		else if(type == 5){ // Create arrows
			npc.addText("So you want to make the arrow? True, the better the arrow, the bigger advantage you have in a battle... Alright, what kind?");
			npc.addText("#b\r\n#L26#Arrow for Bow#l\r\n#L27#Arrow for Crossbow#l\r\n#L28#Bronze Arrow for Bow#l\r\n#L29#Bronze Arrow for Crossbow#l#k");
			npc.sendSimple();
		}
	}
	else if(state == 3){
		if(type == 0){ // War Bow
			npc.addText("To make one War Bow, I need the following materials. The Level limit is 10 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\n5 Processed Woods\r\n30 Blue Snail Shells\r\n800 mesos#k");
			npc.setState(299);
			npc.sendYesNo();
		}
		else if(type == 1){ // Composite Bow
			npc.addText("To make one Composite bow, I need the following materials. The level limit is 15 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\nSteel Plate\r\n3 Screws\r\n2000 mesos#k");
			npc.setState(300);
			npc.sendYesNo();
		}
		else if(type == 2){ // Hunter's Bow
			npc.addText("To make one Hunter's Bow, I need the following materials. The Level limit is 20 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\n30 Processed Woods\r\n50 Red Snail Shells\r\n3000 mesos#k");
			npc.setState(301);
			npc.sendYesNo();
		}
		else if(type == 3){ // Battle Bow
			npc.addText("To make one Battle Bow, I need the following materials. The Level limit is 25 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\n2 Steel Plates\r\n2 Topazs\r\n8 Screws\r\n5000 mesos#k");
			npc.setState(302);
			npc.sendYesNo();
		}
		else if(type == 4){ // Ryden
			npc.addText("To make one Ryden, I need the following materials. The Level limit is 30 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\n5 Steel Plates\r\n5 Gold Plates\r\n3 Emeralds\r\n2 Topazs\r\n30 Screws\r\n30000 mesos#k");
			npc.setState(303);
			npc.sendYesNo();
		}
		else if(type == 5){ // Red Viper
			npc.addText("To make one Red Viper, I need the following materials. The Level limit is 35 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\n7 Silver Plates\r\n6 Garnets\r\n3 Opals\r\n35 Screws\r\n40000 mesos#k");
			npc.setState(304);
			npc.sendYesNo();
		}
		else if(type == 6){ // Vaulter 2000
			npc.addText("To make one Vaulter, I need the following materials. The Level limit is 40 and please make sure you don't use an item that's been upgraded as a material for it. What do you think? Do you want one?");
			npc.addText("#b\r\nBlack Crystal\r\n10 Steel Plates\r\n3 Gold Plates\r\n50 Drake Skulls\r\n40 Screws\r\n80000 mesos#k");
			npc.setState(305);
			npc.sendYesNo();
		}
	}
	else if(state == 300){ // War Bow Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4003001) < 5 || player.getItemAmount(4000000) < 30 || player.getMesos() < 800){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-800);
				player.giveItem(4003001, -5);
				player.giveItem(4000000, -30);
				player.giveItem(1452002, 1);
				npc.addText("Hey! Here, take the War Bow. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 301){ // Composite Bow Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4011001) < 1 || player.getItemAmount(4003000) < 3 || player.getMesos() < 2000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-2000);
				player.giveItem(4011001, -1);
				player.giveItem(4003000, -3);
				player.giveItem(1452003, 1);
				npc.addText("Hey! Here, take the Composite Bow. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 302){ // Hunter's Bow Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4003001) < 30 || player.getItemAmount(4000016) < 50 || player.getMesos() < 3000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-3000);
				player.giveItem(4003001, -30);
				player.giveItem(4000016, -50);
				player.giveItem(1452001, 1);
				npc.addText("Hey! Here, take the Hunter's Bow. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 303){ // Battle Bow Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4011001) < 2 || player.getItemAmount(4021006) < 2 || player.getItemAmount (4003000) < 8 || player.getMesos() < 5000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-5000);
				player.giveItem(4011001, -2);
				player.giveItem(4021006, -2);
				player.giveItem(4003000, -8);
				player.giveItem(1452000, 1);
				npc.addText("Hey! Here, take the Battle Bow. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 304){ // Ryden Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4011001) < 5 || player.getItemAmount(4011006) < 5 || player.getItemAmount (4021003) < 3 || player.getItemAmount (4021006) < 2 || player.getItemAmount (4003000) < 30 || player.getMesos() < 30000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-30000);
				player.giveItem(4011001, -5);
				player.giveItem(4011006, -5);
				player.giveItem(4021003, -3);
				player.giveItem(4021006, -2);
				player.giveItem(4003000, -30);
				player.giveItem(1452005, 1);
				npc.addText("Hey! Here, take the Ryden. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 305){ // Red Viper Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4011004) < 7 || player.getItemAmount(4021000) < 6 || player.getItemAmount (4021004) < 3 || player.getItemAmount (4003000) < 35 || player.getMesos() < 40000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-40000);
				player.giveItem(4011004, -7);
				player.giveItem(4021000, -6);
				player.giveItem(4021004, -3);
				player.giveItem(4003000, -35);
				player.giveItem(1452006, 1);
				npc.addText("Hey! Here, take the Red Viper. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 306){ // Vaulter 2000 Complete
		if(npc.getSelected() == YES){
			if(player.getItemAmount(4021008) < 1 || player.getItemAmount(4011001) < 10 || player.getItemAmount (4011006) < 3 || player.getItemAmount (4000014) < 50 || player.getItemAmount (4003000) < 40 || player.getMesos() < 80000){
				npc.addText("Check and see if you have everything you need and if your equipment inventory may be full or not.");
				npc.sendOK();
				npc.end();
			}
			else{
				player.giveMesos(-80000);
				player.giveItem(4021008, -1);
				player.giveItem(4011001, -10);
				player.giveItem(4011006, -3);
				player.giveItem(4000014, -50);
				player.giveItem(4003000, -40);
				player.giveItem(1452007, 1);
				npc.addText("Hey! Here, take the Red Viper. There was a little hiccup midway through making this, but... it came out to be quite something. Please find me if you need something else down the road. Til then, goodbye...");
				npc.end();
			}
		}
		else{
			npc.addText("You don't have the materials? Please come back later. It looks like I'll be staying here for a while");
			npc.sendOK();
			npc.end();
		}
	}
}