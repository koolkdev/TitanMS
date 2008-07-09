void npc_main(NPC @npc){
	int state = npc.getState();
	if(state == 0){
		npc.addText("How easy is it to consume the item? Simple, right? You can set a #bhotkey#k on the right bottom slot. Haha you didn't know that! right? ");
		npc.addText("Oh, and if you are a begineer, HP will automatically recover itself as time goes by. Well it takes time but this is one of the strategies for the beginners.");
		npc.sendNext();
	}
	else if(state == 1){
		npc.addText("Alright! Now that you have learned alot, I will give you a present. This is a must for your travle in Maple World, so thank me! Please use this under emergency cases!");
		npc.sendBackNext();
	}
	else if(state == 2){
		npc.addText("Okay, this is all I can teach you. I know it's sad but it is time to say good bye. Well tack care of yourself and Good luck my friend!\r\n\r\n");
		npc.addText("#fUI/UIWindow.img/QuestIcon/4/0#\r\n#v2010000# 3 #t2010000#\r\n#v2010009# 3 #t2010009#\r\n\r\n#fUI/UIWindow.img/QuestIcon/8/0# 10 exp");	
		npc.sendBackNext();
	}
	else if(state == 3){
		npc.endQuest(1021);
		npc.giveItem(2010000, 3);
		npc.giveItem(2010009, 3);
		npc.giveEXP(10);
		npc.end();
		
	}
}