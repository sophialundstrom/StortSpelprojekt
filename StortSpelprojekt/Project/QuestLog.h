#pragma once
#include "Singleton.h"
#include "Player.h"
#include "QuestLogFile.h"
#include <map>

class QuestLog : public Singleton<QuestLog>
{
private:
	std::shared_ptr<Player> player;
	std::map<UINT, Quest*> quests;
	std::vector<Quest*> activeQuests;

	void ActivateTriggerQuests(Quest* quest)
	{
		const auto& newQuests = quest->GetTriggerQuests();
		for (auto& ID : newQuests)
			Activate(ID);
	}

	void AppendQuest(Quest* quest)
	{
		auto found = std::find(activeQuests.begin(), activeQuests.end(), quest);
		if (found == activeQuests.end())
			activeQuests.emplace_back(quest);
	}

	void EraseQuest(Quest* quest)
	{
		for (UINT i = 0; i < activeQuests.size(); ++i)
			if (activeQuests[i] == quest)
				activeQuests.erase(activeQuests.begin() + i);
	}
public:
	QuestLog(const std::string& name, std::shared_ptr<Player> player)
		:Singleton(this), player(player)
	{
		QuestLogFile::Load(name, quests);

		for (auto& [ID, quest] : quests)
			if (quest->IsActive())
				AppendQuest(quest);
	}

	void Save(const std::string& name)
	{
		QuestLogFile::Save(name, quests);
	}

	~QuestLog()
	{
		for (auto& [ID, quest] : quests)
			delete quest;
	}

	void Update()
	{
		for (auto* quest : activeQuests)
		{
			quest->Update(player);

			if (quest->IsCompleted())		//IF COMPLETED BY AUTOMATIC REASON (COLLECTING/FIGHT)
			{
				ActivateTriggerQuests(quest);
				EraseQuest(quest);
			}
		}
	}

	//TO BE ABLE TO START A QUEST FROM GAME
	void Activate(UINT ID)
	{
		if (quests[ID]->IsCompleted() || quests.find(ID) == quests.end())
			return;

		else
		{
			quests[ID]->Activate(player);
			AppendQuest(quests[ID]);
		}	
	}

	//TO BE ABLE TO COMPLETE A QUEST FROM GAME
	void Complete(UINT ID)
	{
		for (UINT i = 0; i < activeQuests.size(); ++i)
		{
			Quest* quest = activeQuests[i];

			if (quest->GetID() == ID)
			{
				quest->Complete();
				ActivateTriggerQuests(quest);
				EraseQuest(quest);
			}
		}
	}

	void RenderUI()
	{
		//RENDER UI FOR ALL ACTIVE QUESTS (SHOWING PROGRESS AND SUCH)
		for (auto& [ID, quest] : quests)
			quest->RenderUI();
	}

	bool QuestIsDone(UINT ID)
	{
		return quests[ID]->IsCompleted();
	}
};