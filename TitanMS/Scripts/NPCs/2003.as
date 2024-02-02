/* Made by ShingingDB */
void npc_main(NPC @npc){
	int state = npc.getState();
	int type = npc.getSelected();
	npc.setVariable("type", type);
	if(state == 0){
		npc.addText("Now... ask me any questions you might have on traveling!!\r\n#L0##bHow do I move?#l\r\n#L1#How do I take down the monsters?#l\r\n#L2#How can I pick up an item?#l\r\n#L3#What happens when I die?#l\r\n#L4#When can I choose a job?#l\r\n#L5#Tell me more about this island!#l\r\n#L6#What should I do to become a Warrior?#l\r\n#L7#What should I do to become a Bowman?#l\r\n#L8#What should I do to become a Magician?#l\r\n#L9#What should I do to become a Thief?#l\r\n#L10#How do I raise the character stats? (S)#l\r\n#L11#How do I check the items that I just picked up?#l\r\n#L12#How I put on an item?#l\r\n#L13#How do i check out the items that I'm wearing?#l\r\n#L14#What are skills? (K)#l\r\n#L15#How do I get to Victoria Island?#l\r\n#L16#What are mesos?#l#k");
		npc.sendSimple();
		}
	else if(state == 1){
		if(type == 0){ // How do I move?
			npc.addText("Alright this is how you move. Use #bleft, right arrow#k to move around the flatland and slanted roads, and press #bAlt#k to jump. A selected number of shoes improves you speed and jumping abilities.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 1){ // How do I take down the monsters?
			npc.addText("Here's how to take down a monster. Every monster possesses an HP of its own and you'll take them down by attacking with either a weapon or through spells. Of course the stronger they are, the harder it is to take them down.");
			npc.sendNext();	
		}
		else if(type == 2){ // How can I pick up an item?
			npc.addText("This is how you gather up an item. Once you take down a monster, an item will be dropped to the ground. When that happens, stand in front of the item and press #bZ#k or #b0 on the Numpad#k to acquire the item.");
			npc.sendNext();
		}
		else if(type == 3){ // What happens when I die?
			npc.addText("Curious to find out what happens when you die? You'll become a ghost when your HP reaches 0. There will be a tombstone in that place and you won't be able to move, although you still will be able to chat.");
			npc.sendNext();
		}
		else if(type == 4){ // When can I choose a job?
			npc.addText("When do you get to choose your job? Hahaha, take it easy, my friend. Each job has a requirement set for you to meet. Normally a level between 8 and 10 will do, so work hard.");
			npc.sendNext();
		}
		else if(type == 5){ // Tell me more about this island!
			npc.addText("Want to know about this island? It's called Maple Island and it floats in the air. It's been floating in the sky for a while so the nasty monsters aren't really around. It's a very peaceful island, perfect for beginners!");
			npc.sendNext();
		}
		else if(type == 6){ // What should I do to become a Warrior?
			npc.addText("You want to become a #bWarrior#k? Hmmm, then I suggest you head over to Victoria Island. Head over to a warrior-town called #rPerion#k and see #bDances with Balrog#k. He'll teach you all about become a true warrior. Ohh, and one VERY important thing. You'll need to be atleast level 10 in order to become a warrior!!");
			npc.sendOK();
			npc.end();
		}
		else if(type == 7){ // What should I do to become a Bowman?
			npc.addText("You want to become a #bBowman#k? Hmmm, then I suggest you head over to Victoria Island. Head over to a bowman-town called #rHenesys#k and talk to the beautiful #bAthene Pierce#k and learn the in's and out's of being a bowman. Ohh, and one VERY important thing. You'll need to be atleast level 10 in order to become a bowman!!");
			npc.sendOK();
			npc.end();			
		}
		else if(type == 8){ // What should I do to become a Magician?
			npc.addText("You want to become a #bMagician#k? Hmmm, then I suggest you head over to Victoria Island. Head over to a magician-town called #rEllinia#k, and at the very top lies the Magic Library. Inside you'll meet the head of all wizards, #bGrendel the Really Old#k, who'll teach you everything about become a wizard.");
			npc.sendNext();
			npc.end();
		}
		else if(type == 9){ // What should I do to become a Thief?
			npc.addText("You want to become a #bThief#k? Hmmm, then I suggest you head over to Victoria Island. Head over to a thief-town called #rKerning City#k and on the shadier side of town, you'll see a thief's hideaway. There, you'll meet #bDark Lord#k, who'll teach you everything about being a thief. Ohh, and one VERY important thing. You'll need to be atleast level 10 in order to become a thief!!");
			npc.sendOK();
			npc.end();
		}
		else if(type == 10){ // How do I raise the character stats? (S)
			npc.addText("You want to know how to raise your character's ability stats? First press #bS#k to check out the ability window. Every time you level up, you'll be awarded 5 ability points aka AP's. Assign those AP's to the ability to your choice. It's that simple.");
			npc.sendNext();
		}
		else if(type == 11){ // How do I check the items that I just picked up?
			npc.addText("You want to know how to check out the items you've picked up, huh? When you defeat a monster, it'll drop an item on the ground, and you may press #bZ#k to pick up the item. That item will then be stored in your item inventory, and you can take a look at it by simply pressing #bI#k.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 12){ // How do i put on an item?
			npc.addText("You want to know how to wear the items, right? Press #bI#k to check out your item inventory. Place your mouse cursor on top of an item and double-click on it to put it on your character. If you find yourself unable to wear the item, chances are your character does not meet the level & stat requirements. You can also put on the item by opening the equipment inventory (#bE#k) and dragging the item into it. To take off an item, double-click on the item at the equip.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 13){ // How do I check out the items that I'm wearing?
			npc.addText("You want to check on the equipped items, right? Press #bE#k to open the equipment inventory, where you'll see exactly what you are wearing right at the moment. To take off an item, double-click on the item. The item will then be sent to the item inventory.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 14){ // What are skills? (K)
			npc.addText("The special 'abilities' you get after acquiring a job are called skills. You'll acquire skills that are specifically for that job. You're not at that stage yet, so you don't have any skills yet, but just remember that to check on your skills, press #bK#k to open the skill book. It'll help you down the road.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 15){ // How do I get to Victoria Island?
			npc.addText("You can head over to Victoria Island through a ship ride from Southperry that heads to Lith Harbor. Press #bW#k to see the World Map, and you'll see where you are on the island. Locate Southperry and that's where you'll need to go. You'll also need some mesos for the ride, so you made need to hunt some monsters around here.");
			npc.sendOK();
			npc.end();
		}
		else if(type == 16){ // What are mesos?
			npc.addText("It is the currency used in MapleStory. You may purchase items through mesos. To earn them, you may either defeat the monsters, sell items at the store, or complete quests...");
			npc.sendOK();
			npc.end();
		}
	}
	else if(state == 2){
	if (type == 1){ // How do I take down the monsters?
		npc.addText("In order to attack the monsters, you'll need to be equipped with a weapon. When equipped, press #bCtrl#k to use the weapon. With the right timing, you'll be able to easily take down the monsters.");
		npc.sendBackNext();		
		}
	else if(type == 2){ // How can I pick up an item?
			npc.addText("Remember, though, that if your item inventory is full, you won't be abel to acquire more. So if you have an item that you don't need, sell it so you can make something out of it. The inventory may expand once you make the job advancement.");
			npc.sendBackOK();
			npc.end();
		}
		else if(type == 3){ // What happens when I die?
			npc.addText("There isn't much to lose when you die if you are just a Beginner. Once you have a job, however, it's a different story. You'll lose a portion of your EXP when you die, so make sure you avoid danger and death at all cost.");
			npc.sendBackOK();
			npc.end();
		}
		else if(type == 4){ // When can I choose a job?
			npc.addText("Level isn't the only thing that determines the advancement, though. You also need to boost up the levels of a particular ability based on the occupation. For example, to be a warrior, your STR has to be over 35, and so forth, you know what I'm saying? Make sure you boost up the abilities that has direct implications to your job.");
			npc.sendBackOK();
			npc.end();
		}
		else if(type == 5){ // Tell me more about this island!
			npc.addText("But, if you want to be a powerful player, better not think about staying here for too long. You won't be able to get a job anyway. Underneath this island lies an enormous island called Victoria Island. That place is so much bigger than here, it's not even funny.");
			npc.sendBackNext();
		}
		else if(type == 8){ // What should I do to become a Magician?
			npc.addText("Oh by the way, unlike other jobs, to become a magician, you only need to be at level 8. What comes with making this job advancement early also comes with the fact that it takes a lot to become a true powerful mage. Think long and carefully before choosing your path.");
			npc.sendBackOK();
			npc.end();
		}
		else if(type == 10){ // How do I raise the character stats? (S)
			npc.addText("Place your mouse cursor on top of all abilities for a brief explanation. For example, STR for warriors, DEX for bowman, INT for magician, and LUK for thief. That itself isn't everything you need to know, so you'll need to think long and hard on how to emphasize your character's strengths through assigning the points.");
			npc.sendBackOK();
			npc.end();
		}
	}
	if(state == 3){ 
		if (type == 1){// How do I take down the monsters?
		npc.addText("Once you make the job advancement, you'll acquire different kinds of skills, and you can store them as HotKey's for easier access. If it's an attacking skill, you dont need to press Ctrl to attack, just the button assigned as a HotKey.");
			npc.sendBackOK();
			npc.end();			
		}
		else if(type == 5){ // Tell me more about this island!
			npc.addText("How do you get to Victoria Island? On the east of this island there's a harbor called Southperry. There, you'll find a ship that flies in the air. In front of the ship stands a captain. Ask him about it.");
			npc.sendBackNext();
		}
	}
	if(state == 4){ 
		if(type == 5){ // Tell me more about this island!
			npc.addText("Oh yea! One last piece of information before I go. If you are not sure where you are, always press #bW#k. The world map will pop up with the locator showing where you stand. You won't have to worry about getting lost with that.");
			npc.sendBackOK();
			npc.end();
		}
	}
}