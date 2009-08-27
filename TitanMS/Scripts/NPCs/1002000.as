void npc_main(NPC @npc){
	int state = npc.getState();
	int type = npc.getSelected();
	Player@ player = npc.getPlayer();
	int [] cities = {102000000, 101000000, 100000000, 103000000};
	int [] cost = {1200, 1200, 800, 1000}; 
	if(player.getJob() == 0){
		cost[0] /= 10; cost[1] /= 10; cost[2] /=10; cost[3] /=10;
	}	
	if(state == 0){
		npc.addText("Do you wanna head over to some other town? With a little money involved, I can make it happen. It's a tad expensive, but I run a special 90% discount for beginners.");
		npc.sendNext();
	}
	else if(state == 1){
		npc.addText("It's understable that you may be confused about this place if this is your first time around. If you got any questions about this place, fire away. #b\r\n#L0#What kind of towns are here in Victoria Island?#l\r\n#L1#Please take me somewhere else.#l#k");
		npc.sendSimple();
	}
	else if(state == 2){
		npc.setVariable("type", type);
		if(type == 0){ // What kind of towns are here in Victoria Island?
			npc.addText("There are 6 big towns here in Victoria Island. Which of those do you want to know more of? #b\r\n#L0#Lith Harbor#l\r\n#L1#Perion#l\r\n#L2#Ellinia#l\r\n#L3#Henesys#l\r\n#L4#Kerning City#l\r\n#L5#Sleepywood#l#k");
			npc.sendSimple();
		}
		else{
			npc.addText("There's a special 90% discount for all beginners. Alright, where would you like to go? #b");
			for(int i=0; i<4; i++){
				npc.addText("\r\n#L" + i + "##m" + cities[i] + "#(" + cost[i] + " mesos)#l");
			}
			npc.sendSimple();
		}
	}
	else if(state == 3){
		int city = npc.getSelected();
		npc.setVariable("city", city);
		if(npc.getVariable("type") == 0){
			if(type == 0){  // Lith Harbor
				npc.addText("The town you are at is Lith Harbor! Alright, I'll explain to you more about #bLith Harbor#k. It's the place you landed on Victoria Island by riding The Victoria. That's Lith Harbor. A lot of beginners who just got here from Maple Island start their journey here.");
				npc.sendNext();
			}
			else if(type == 1){ // Perion	
				npc.addText("Alright I'll explain to you more about #bPerion#k. It's a warrior-town located at the northern-most part of Victoria Island, surrounded by rocky mountains. With an unfriendly atmosphere, only the strong survives there.");	
				npc.sendNext();	
			}
			else if(type == 2){// Ellinia
				npc.addText("Alright I'll explain to you more about #bEllinia#k. It's a magician-town located at the far east of Victoria Island, and covered in tall, mystic trees. You'll find more fairies there too. They don't like humans in general, so it'll be best for you to be on their good side and stay quiet.");
				npc.sendNext();
			}
			else if(type == 3){ // Henesys
				npc.addText("Alright I'll explain to you more about #bHenesys#k. It's a bowman-town located at the most southernmost part of the island, made on a flatland in the midst of a deep forest and praries. The weather's just right, and everything is plentiful around that town, perfect for living. Go check it out.");
				npc.sendNext();
			}
			else if(type == 4){ // Kerning City
				npc.addText("Alright I'll explain to you more about #bKerning City#k. It's a thief-town located at the northwest part of Victoria Island, and there are buildings up there that have just this strange feeling around them. It's mostly covered in black clouds, but if you can go up to a really high place, you'll be able to see a very beautiful sunset there.");
				npc.sendNext();
			}
			else if(type == 5){ // Sleepywood
				npc.addText("Alright I'll explain to you more about #bSleepywood#k. It's a forest town located at the southeast side of Victoria Island. It's pretty much in between Henesys and the ant-tunnel dungeon. There's a hotel there, so you can rest up after a long day at the dungeon. It's a quiet town in general.");
				npc.sendNext();
			}
		}
		else{ // Please take me somewhere else.
			npc.addText("I guess you don't need to be here. Do you really want to move to #b#m" + cities[city] + "##k? Well it'll cost you #b" + cost[city] + " meso#k. What do you think?");
			npc.sendYesNo();
		}

	}
	else if(state == 4){
		type = npc.getVariable("city");
		if(npc.getVariable("type") == 0){
			if(type == 0){ // Lith Harbor
				npc.addText("It's a quiet town with the wide body of water on the back of it, thanks of the fact that the harbor is located at the west end of the island. Most of the people here are, or used to be fishermen, so they may look intimidating, but if you strike up a conversation with them, they'll be friendly to you.");
				npc.sendBackNext();
			}
			else if(type == 1){ // Perion
				npc.addText("Around the highland, you'll find a really skinny tree, a wild hog running around the place, and monkeys that live all over the island. There's also a deep valley, and when you go deep into it, you'll find a humongous dragon with the power to match his size. Better go in there very carefully, or don't go at all.");
				npc.sendBackNext();
			}
			else if(type == 2){ // Ellinia
				npc.addText("Near the forest, you'll find green slimes, walking mushrooms, monkeys, and zombie monkeys all residing there. Walk deeper into the forest and you'll find witches with the flying broomstick navigating the skies. A word of warning, unless you are really strong, I recommend you don't go near them.");
				npc.sendBackNext();
			}
			else if(type == 3){ // Henesys
				npc.addText("Around the prairie, you'll find weak monsters such as snails, mushrooms, and pigs. According to what I hear, though, in the deepest part of the Pig Park, which is connected to the town somewhere, you'll find a humongous, powerful mushroom called Mushmom every now and then.");
				npc.sendBackNext();
			}
			else if(type == 4){ // Kerning City
				npc.addText("From Kerning City, you can go into serveral dungeons. You can go to a swamp where alligators and snakes are abound, or hit the subway full of ghosts and bats. At the deepest part of the underground, you'll find Lace, who is just as big and dungerous as a dragon.");
				npc.sendBackNext();
			}
			else if(type == 5){ // Sleepywood
				npc.addText("In front of the hotel, there's an old Buddhist Monk by the name of #rChrishrama#k. Nobody knows a thing about that monk. Apparently he collects materials from the travellers and creates something, but I am not too sure about the details. If you have any business going around that area, please check that out for me.");
				npc.sendBackNext();
			}
		}
		else{
			if(npc.getSelected() == YES){
				if(player.getMesos() < cost[type]){
					npc.addText("You don't have enough mesos. With your abilities, you should have more than that!");
	                   			npc.sendOK();
				}
				else {
					player.giveMesos(-cost[type]);
					player.changeMap(cities[type]);
				}
			}
			else{
				npc.addText("There's alot to see in this town too. Let me know if you want to go somewhere else.");
				npc.sendOK();		
			}
			npc.end();
		}
	}
	else if(state == 5){
		if(type == 0){ // Lith Harbor
			npc.addText("Around town lies a beautiful prarie. Most of the monsters there are small and gentle, perfect for beginners. If you haven't chosen your job yet, this is a good place to boost up your level.");
			npc.sendBackNext();
			npc.end();
		}
		else if(type == 1){ // Perion
			npc.addText("If you want to be a #bWarrior#k, then find #rDances with Balrog#k, the chief of Perion. If you're level 10 or higher, along with a good STR level, he might make you a warrior afterall. If not, better keep training yourself until you reach that level.");
			npc.sendBackNext();
			npc.end();
		}
		else if(type == 2){ // Ellinia
			npc.addText("If you want to be a #bMagician#k, search for #rGrendel the Really Old#k, the head wizard of Ellinia. He may make you a wizard if you're at or above level 8 with a decent amount of INT. If that's not the case, you may have to hunt more and train yourself to get there.");
			npc.sendBackNext();
			npc.end();
		}
		else if(type == 3){ // Henesys
			npc.addText("If you want to be a #bBowman#k, you need to go see #rAthena Pierce#k at Henesys. With a level at or above 10 and a decent amount of DEX, she might make you be one afterall. If not, go train yourself, make yourself stronger, then try again.");
			npc.sendBackNext();
			npc.end();
		}
		else if(type == 4){ // Kerning City
			npc.addText("If you want to be a #bThief#k, seek #rDark Lord#k, the heart of darkness of Kerning City. He might well make you a thief if you're at or above level 10 with a good amount of DEX. If not, go hunt and train to reach there.");
			npc.sendBackNext();
			npc.end();
		}
		else if(type == 5){ // Sleepywood
			npc.addText("From Sleepywood, head east and you'll find the ant tunnel connected to the deepest part of Victoria Island. Lots of nasty, powerful monsters abound, so if you walk in thinking it's a walk in the park, you'll be coming out as a corpse. You need to fully prepare yourself for a rough riding before going in.");
			npc.sendBackNext();
		}
	}
	if(state == 6){
		if(type == 5){ // Sleepywood
			npc.addText("And this is what I hear, apparently, at Sleepywood there's a secret entrance leading you to an unknown place. Apparently, once you move in deep, you'll find a stack of black rocks that actually move around. I want to see that for myself in the near future...");
			npc.sendBackNext();
			npc.end();
		}
	}

}