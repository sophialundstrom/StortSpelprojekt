#include "QuestLog.h"

void QuestLog::Update(std::shared_ptr<Player> player, std::vector<BarbarianCamp> camps, std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs, std::vector<std::shared_ptr<Target>> targets)
{
	for (UINT i = 0; i < activeQuests.size(); ++i)
	{
		auto quest = activeQuests[i];

		quest->Update(player, camps, friendlyNPCs, targets);

		if (quest->IsCompleted())
		{
			activeQuests.erase(activeQuests.begin() + i);

			for (auto child : quest->GetChildQuests())
			{
				activeQuests.emplace_back(child);
				child->Activate();
			}
		}
	}

	//UPDATE UI
}

void QuestLog::Save(const std::string& fileName)
{
	File file(fileName, false, false);

	if (!file.OutputIsOpen())
		return;

	for (auto quest : quests)
		quest->SaveToFile(file);

	file.Close();
}

void QuestLog::Load(const std::string& fileName)
{
	File file(fileName, true, false);

	if (!file.InputIsOpen())
		return;

	while (!file.EndOfFile())
	{
		auto quest = new Quest();
		quest->LoadFromFile(file);

		quests.emplace_back(quest);

		if (quest->IsActive())
			activeQuests.emplace_back(quest);

		for (auto child : quest->GetChildQuests())
		{
			quests.emplace_back(child);
			if (child->IsActive())
				activeQuests.emplace_back(child);
		}
	}

	file.Close();
}

void QuestLog::CreateQuests()
{
	//CREATE THE QUESTS HERE, START ONCE TO CREATE THE FILE

	//BEFORE FIRST QUEST, A LIL MESSAGE TO PLAYER LIKE "SEEMS LIKE >>NPC<< HAS A QUEST FOR YOU, YOU SHOULD TALK TO HIM ( WILL BE THE ONLY ONE WITH A SYMBOL )

	//=====================================================
	//NPC1
	auto q1 = new Quest("A Helping Hand.", true, false);		//COLLECTING A HAMMER ( MAYBE TO FINISH BUILDING BOW HUT OR SMTH)
	q1->AddCollectObjective(Item::Type::Hammer, 1);
	
	auto q2 = q1->AddChildQuest("Target Aquired.");				//SHOOT THREE TARGETS WITH GIVEN ARROWS FROM LAST QUEST
	q2->AddTargetObjective(0);
	q2->AddTargetObjective(1);
	q2->AddTargetObjective(2);

	auto q3 = q2->AddChildQuest("We're Under Attack!");			//FIGHT THE BARBARIANS ATTACKING THE VILLAGE
	q3->AddFightObjective(BarbarianCamp::Location::Village);

	//=====================================================
	//NPC2


	//AFTER QUESTS ARE MADE (OR READ FROM EXISTING FILE) ASSIGN TO NPCs
}

void QuestLog::ShutDown()
{
	for (auto quest : quests)
		delete quest;
}