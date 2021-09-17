#pragma once
#include "Singleton.h"
#include "Player.h"
#include "TalkQuest.h"
#include "CollectQuest.h"
#include "FightQuest.h"
#include <map>

class QuestLog : public Singleton<QuestLog>
{
private:
	Player* player;
	std::map<unsigned int, Quest*> quests;
	std::vector<Quest*> activeQuests;
public:
	QuestLog(Player* player)
		:Singleton(this), player(player)
	{
		//READ FILE WITH QUESTS ?
	}

	void Update()
	{
		for (auto* quest : activeQuests)
			quest->Update();
	}

	void Activate(unsigned int ID)
	{
		quests[ID]->Activate();
	}

	void Complete(unsigned int ID)
	{
		quests[ID]->Complete();
		activeQuests.erase(activeQuests.begin() + ID);
	}

	void RenderUI()
	{
		//RENDER UI FOR ALL ACTIVE QUESTS (SHOWING PROGRESS AND SUCH)
		for (auto& [ID, quest] : quests)
			if (quest->IsActive())
				quest->RenderUI();
	}
};