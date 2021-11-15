#include "Quest.h"

#include "NPCFriendly.h"
#include "Target.h"

class QuestLog
{
private:
	static std::vector<Quest*> quests;
	static std::vector<Quest*> activeQuests;
public:
	static void Update(std::shared_ptr<Player> player, std::vector<BarbarianCamp> camps, std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs, std::vector<std::shared_ptr<Target>> targets);
	static void Save(const std::string& fileName);
	static void Load(const std::string& fileName);
};

//#pragma once
//#include "Singleton.h"
//#include "Player.h"
//#include "QuestLogFile.h"
//#include "Canvas.h"
//#include <map>
//
//class QuestLog : public Singleton<QuestLog>
//{
//private:
//	std::shared_ptr<Player> player;
//
//	//CANVAS 
//	std::shared_ptr<Canvas> ingameCanvas;
//	const UINT UIStartOffset = 100u;
//	const UINT UILeftOffset = 70u;
//	const UINT UIOffset = 80u;
//
//	bool IsQuestDone = false;
//
//	std::map<UINT, Quest*> quests;
//	std::vector<Quest*> activeQuests;
//
//	void ActivateTriggerQuests(Quest* quest)
//	{
//		const auto& newQuests = quest->GetTriggerQuests();
//		for (auto& ID : newQuests)
//			Activate(ID);
//	}
//
//	void AppendQuest(Quest* quest)
//	{
//		auto found = std::find(activeQuests.begin(), activeQuests.end(), quest);
//		if (found == activeQuests.end())
//			activeQuests.emplace_back(quest);
//	}
//
//	void EraseQuest(Quest* quest)
//	{
//		for (UINT i = 0; i < activeQuests.size(); ++i)
//			if (activeQuests[i] == quest)
//			{
//				ingameCanvas->RemoveText(quest->Name() + "title");
//				ingameCanvas->RemoveText(quest->Name() + "text");
//				activeQuests.erase(activeQuests.begin() + i);
//			}
//				
//	}
//
//	void UpdateUI()
//	{
//		static UINT numQuests = 0;
//
//		for (UINT i = 0; i < activeQuests.size(); ++i)
//		{
//			auto quest = activeQuests[i];
//
//			if (!ingameCanvas->Exists<Text>(quest->Name() + "title")) // ADDING NEW QUEST (NO OTHER REMOVED)
//			{
//				ingameCanvas->AddText({ (float)UILeftOffset, (float)UIStartOffset + (UIOffset * i) }, quest->Name() + "title", quest->Name(), UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_SMALL);
//				std::string text;
//				quest->UpdateUI(text);
//				ingameCanvas->AddText({ (float)UILeftOffset, (float)UIStartOffset +(UIOffset * i) + 30 }, quest->Name() + "text", text, UI::COLOR::YELLOW, UI::TEXTFORMAT::DEFAULT);
//			}
//
//			else if (numQuests != activeQuests.size()) //OFFSET EXISTING QUEST IF ONE IS REMOVED
//			{
//				std::string text;
//				quest->UpdateUI(text);
//				ingameCanvas->UpdateText(quest->Name() + "text", text);
//				ingameCanvas->GetText(quest->Name() + "title")->SetLeftSidePosition(UILeftOffset, UIStartOffset + (UIOffset * i));
//				ingameCanvas->GetText(quest->Name() + "text")->SetLeftSidePosition(UILeftOffset, UIStartOffset + (UIOffset * i) + 30);
//			}
//
//			else //UPDATE QUEST STRING
//			{
//				std::string text;
//				quest->UpdateUI(text);
//				ingameCanvas->UpdateText(quest->Name() + "text", text);
//			}
//		}
//
//		numQuests = (UINT)activeQuests.size();
//	}
//public:
//	QuestLog(const std::string& name, std::shared_ptr<Player> player, std::shared_ptr<Canvas> ingameCanvas)
//		:Singleton(this), player(player), ingameCanvas(ingameCanvas)
//	{
//		QuestLogFile::Load(name, quests);
//
//		for (auto& [ID, quest] : quests)
//			if (quest->IsActive())
//				AppendQuest(quest);
//	}
//
//	void Save(const std::string& name)
//	{
//		QuestLogFile::Save(name, quests);
//	}
//
//	~QuestLog()
//	{
//		for (auto& [ID, quest] : quests)
//			delete quest;
//	}
//
//	void Update()
//	{
//		for (auto* quest : activeQuests)
//		{
//			quest->Update(player);
//
//			if (quest->IsCompleted())		//IF COMPLETED BY AUTOMATIC REASON (COLLECTING/FIGHT)
//			{
//				/*ingameCanvas->UpdateText("WOOD", std::to_string(player->Inventory().NumOf(RESOURCE::WOOD)));
//				ingameCanvas->UpdateText("STONE", std::to_string(player->Inventory().NumOf(RESOURCE::STONE)));
//				ingameCanvas->UpdateText("FOOD", std::to_string(player->Inventory().NumOf(RESOURCE::FOOD)));*/
//				ActivateTriggerQuests(quest);
//				EraseQuest(quest);
//			}
//		}
//
//		UpdateUI();
//	}
//
//	//TO BE ABLE TO START A QUEST FROM GAME
//	void Activate(UINT ID)
//	{
//		if (quests.find(ID) == quests.end() || quests[ID]->IsCompleted())
//			return;
//
//		else
//		{
//			quests[ID]->Activate(player);
//			AppendQuest(quests[ID]);
//		}	
//	}
//
//	//TO BE ABLE TO COMPLETE A QUEST FROM GAME
//	void Complete(UINT ID)
//	{
//		for (UINT i = 0; i < activeQuests.size(); ++i)
//		{
//			Quest* quest = activeQuests[i];
//
//			if (quest->GetID() == ID)
//			{
//				quest->Complete();
//				ActivateTriggerQuests(quest);
//				UpdateUI();
//				EraseQuest(quest);
//			}
//		}
//	}
//
//	bool QuestIsActive(UINT ID)
//	{
//		for (auto& quest : activeQuests)
//			if (quest->GetID() == ID)
//				return true;
//
//		return false;
//	}
//
//	bool QuestIsDone(UINT ID)
//	{
//		if (quests.find(ID) == quests.end())
//		{
//			Print("QUEST ID NOT FOUND");
//			return false;
//		}
//			
//		return quests[ID]->IsCompleted();
//	}
//
//	bool GetActiveQuest() { return this->activeQuests.size(); }
//};