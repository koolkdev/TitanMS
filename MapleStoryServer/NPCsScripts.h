#ifndef NPCSSCRIPT_H
#define NPCSSCRIPT_H

class NPC;

int strval(char* buf);

class QuestsScripts {
public:
	static void handle(int npcid, NPC* npc, bool start){
		if(start){
			switch(npcid){
				case 2000: npc_2000s(npc); break;
				default: npc->end(); break;
			}
		}
		else{
			switch(npcid){
				case 2000: npc_2000e(npc); break;
				default: npc->end(); break;
			}
		}
	}
private:
	static void npc_2000s(NPC* npc);
	static void npc_2000e(NPC* npc);
};

class NPCsScripts {
public:
	static void handle(int npcid, NPC* npc){
		if(npc->isQuest()){
			QuestsScripts::handle(npcid, npc, npc->isStart());
			return;
		}
		switch(npcid){
			case 2100: npc_2100(npc); break;
			case 2101: npc_2101(npc); break;
			case 2020005: npc_2020005(npc); break;
			case 9101001: npc_9101001(npc); break;
			case 9900000: npc_9900000(npc); break;
			default: npc->end(); break;
		}
	}
private:
	static void npc_2100(NPC* npc);
	static void npc_2101(NPC* npc);
	static void npc_2020005(NPC* npc);
	static void npc_9101001(NPC* npc);
	static void npc_9900000(NPC* npc);
};

#endif