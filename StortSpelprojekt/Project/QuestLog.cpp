#include "QuestLog.h"

void QuestLog::LoadQuest(Quest* quest)
{
	quests.emplace_back(quest);

	if (quest->IsActive())
		activeQuests.emplace_back(quest);

	for (auto child : quest->GetChildQuests())
		LoadQuest(child);
}

void QuestLog::Update(std::shared_ptr<Player> player, std::vector<BarbarianCamp> camps, std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs, std::vector<std::shared_ptr<Target>> targets)
{
	for (UINT i = 0; i < activeQuests.size(); ++i)
	{
		auto quest = activeQuests[i];
		if (!quest->Unlocked())
			continue;

		quest->Update(player, camps, friendlyNPCs, targets);

		/*if (quest->IsCompleted())
		{
			activeQuests.erase(activeQuests.begin() + i);

			for (auto child : quest->GetChildQuests())
			{
				activeQuests.emplace_back(child);
				child->Activate();
			}
		}*/
	}

	if (Event::KeyIsPressed('O'))
	{
		Print("\n>>>>>>>>>>>>>>> CURRENT QUESTS <<<<<<<<<<<<<<");

		for (auto quest : activeQuests)
		{
			if (!quest->Unlocked())
				continue;

			Print(quest->GetName(), "==================");
			for (auto objective : quest->GetObjectives())
				Print(objective->Info());
		}
	}

	//UPDATE UI
}

void QuestLog::Save(const std::string& fileName)
{
	File file(FileSystem::ProjectDirectory::path + "\\SaveData\\" + fileName + ".qsl", false);

	if (!file.OutputIsOpen())
		return;

	for (auto quest : quests)
		quest->SaveToFile(file);

	file.Close();
}

void QuestLog::Load(const std::string& fileName)
{
	File file(FileSystem::ProjectDirectory::path + "\\SaveData\\" + fileName + ".qsl", true);

	if (!file.InputIsOpen())
		return;

	while (!file.EndOfFile())
	{
		auto quest = new Quest();
		quest->LoadFromFile(file);
		LoadQuest(quest);
	}

	file.Close();
}

void QuestLog::CreateQuests()
{
	//CREATE THE QUESTS HERE, START ONCE TO CREATE THE FILE

	//BEFORE FIRST QUEST, A LIL MESSAGE TO PLAYER LIKE "SEEMS LIKE >>NPC<< HAS A QUEST FOR YOU, YOU SHOULD TALK TO HIM ( WILL BE THE ONLY ONE WITH A SYMBOL )

	//=====================================================
	//NPC1
	auto q1 = new Quest("A Helping Hand.", false, false, true);		//COLLECTING A HAMMER (MAYBE TO FINISH BUILDING BOW HUT OR SMTH)
	q1->AddCollectObjective(Item::Type::Hammer, 1);
	quests.emplace_back(q1);
	
	auto q2 = q1->AddChildQuest("Target Aquired.");				//SHOOT THREE TARGETS WITH GIVEN ARROWS FROM LAST QUEST
	q2->AddTargetObjective(0);
	q2->AddTargetObjective(1);
	q2->AddTargetObjective(2);
	quests.emplace_back(q2);

	auto q3 = q2->AddChildQuest("We're Under Attack!");			//FIGHT THE BARBARIANS ATTACKING THE VILLAGE
	q3->AddFightObjective(BarbarianCamp::Location::Village);
	quests.emplace_back(q3);

	//=====================================================
	//NPC2
	auto q4 = q3->AddChildQuest("FIRST QUEST FOR NPC2");
	quests.emplace_back(q4);
	//SAME STUFF HERE KINDA I THIK U GET IT, IF ANY QUEST DEPENDS ON ANOTHER (CANT BE TWO) YOU HAVE TO CREATE THE FIRST ONE FIRST, 
	//AND ALL QUESTS THAT SHOULD BE "UNLOCKED" AFTER THE ATTACK SHOULD BE CHILD OF Q3

	Save("Default");

	//ShutDown();

	//Load("Default");
	//AFTER QUESTS ARE MADE (OR READ FROM EXISTING FILE) ASSIGN TO NPCs
}

void QuestLog::ShutDown()
{
	for (auto quest : quests)
		delete quest;
}

void QuestLog::Activate(Quest* quest)
{
	for (auto q : activeQuests)
		if (q == quest)
			return;

	quest->Activate();
	activeQuests.emplace_back(quest);
}

void QuestLog::Complete(Quest* quest)
{
	if (!quest->IsCompleted() || !quest->IsActive())
		return;

	for (UINT i = 0; i < activeQuests.size(); ++i)
	{
		if (activeQuests[i] == quest)
		{
			activeQuests.erase(activeQuests.begin() + i);
			break;
		}
	}

	for (auto child : quest->GetChildQuests())
		child->Unlock();
}

Quest* QuestLog::Get(const std::string& name)
{
	for (auto quest : quests)
	{
		if (quest->GetName() == name)
			return quest;
	}

	return nullptr;
}