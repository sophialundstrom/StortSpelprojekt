#pragma once
#include "Singleton.h"
#include "Player.h"
#include "QuestLogFile.h"
#include "Canvas.h"
#include <map>

class QuestLog : public Singleton<QuestLog>
{
private:
	std::shared_ptr<Player> player;

	//CANVAS 
	Canvas* ingameCanvas;
	float titlePositions[5] = { 100.0f, 200.0f, 300.0f, 400.0f, 500.0f };

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
			{
				ingameCanvas->RemoveText(activeQuests[i]->Name() + "title");
				ingameCanvas->RemoveText(activeQuests[i]->Name() + "text");
				activeQuests.erase(activeQuests.begin() + i);
			}	
	}

	void UpdateUI()
	{
		for (UINT i = 0; i < activeQuests.size(); ++i)
		{
			auto quest = activeQuests[i];

			if (!ingameCanvas->Exists<Text>(quest->Name() + "title"))
			{
				ingameCanvas->AddText({ 170, titlePositions[i] }, quest->Name() + "title", quest->Name(), 250, 50, UI::COLOR::GRAY, UI::TEXTFORMAT::TITLE_SMALL);
				std::string text;
				quest->UpdateUI(text);
				ingameCanvas->AddText({ 170, titlePositions[i] + 20 }, quest->Name() + "text", text, 250, 50, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);
			}

			else
			{
				std::string text;
				quest->UpdateUI(text);
				ingameCanvas->UpdateText(quest->Name() + "text", text);
			}
		}
	}
public:
	QuestLog(const std::string& name, std::shared_ptr<Player> player, Canvas* ingameCanvas)
		:Singleton(this), player(player), ingameCanvas(ingameCanvas)
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
				ingameCanvas->UpdateText("Wood", std::to_string(player->Inventory().NumOf(WOOD)));
				ingameCanvas->UpdateText("Stone", std::to_string(player->Inventory().NumOf(STONE)));
				ingameCanvas->UpdateText("Food", std::to_string(player->Inventory().NumOf(FOOD)));
				ActivateTriggerQuests(quest);
				EraseQuest(quest);
			}
		}

		UpdateUI();
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

	bool QuestIsDone(UINT ID)
	{
		if (quests.find(ID) == quests.end())
		{
			Print("QUEST ID NOT FOUND");
			return false;
		}
			
		return quests[ID]->IsCompleted();
	}
};