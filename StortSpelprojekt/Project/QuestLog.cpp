#include "QuestLog.h"

void QuestLog::LoadQuest(Quest* quest)
{
	quests.emplace_back(quest);

	if (quest->IsActive())
		activeQuests.emplace_back(quest);

	for (auto child : quest->GetChildQuests())
		LoadQuest(child);
}

void QuestLog::Update(std::shared_ptr<Player> player, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets, std::vector<std::shared_ptr<FriendlyNPC>> frendlyNPCs)
{
	for (UINT i = 0; i < activeQuests.size(); ++i)
	{
		auto quest = activeQuests[i];
		if (!quest->Unlocked())
			continue;

		quest->Update(player, camps, targets, frendlyNPCs);
	}
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
	auto q1 = new Quest("Getting Started", false, false, true);		//COLLECTING A ROPE (MAYBE TO FINISH BUILDING BOW HUT OR SMTH)
	q1->AddCollectObjective(Item::Type::Rope, 1);
	quests.emplace_back(q1);
	
	auto q2 = q1->AddChildQuest("Sticks And Stones");				//SHOOT THREE TARGETS WITH GIVEN ARROWS FROM LAST QUEST
	q2->AddCollectObjective(Item::Type::Stick, 5);
	q2->AddCollectObjective(Item::Type::Stone, 5);
	quests.emplace_back(q2);

	auto q3 = q2->AddChildQuest("Target Aquired");			//FIGHT THE BARBARIANS ATTACKING THE VILLAGE
	q3->AddTargetObjective(0);
	q3->AddTargetObjective(1);
	q3->AddTargetObjective(2);
	quests.emplace_back(q3);

	auto q4 = q3->AddChildQuest("Invasion!");
	q4->AddFightObjective(BarbarianCamp::Location::Village);
	quests.emplace_back(q4);

	auto q5 = q4->AddChildQuest("Getting Acquainted");
	q5->AddTalkObjective("Ulfric", "Hello there old friend! Grea’ to see tha’ you have made it back to us during these challenging times. Lemme know when you are free, I'm gonna need your help later on.");
	q5->AddTalkObjective("Sven", " Greetings! This is all that remains of my old farm… The barbarians destroyed my entire property so I would really need your help with building it all up again. When you have the time look me up.");
	quests.emplace_back(q5);

	auto q6 = q5->AddChildQuest("Payback");
	q6->AddFightObjective(BarbarianCamp::Location::East);
	quests.emplace_back(q6);

	//=====================================================
	//NPC2
	auto q7 = q5->AddChildQuest("Fetch Me' Hammer");
	q7->AddCollectObjective(Item::Type::Hammer, 1);
	quests.emplace_back(q7);

	auto q8 = q7->AddChildQuest("Spy In The Making");
	q8->AddLocationObjective({ -729.f, 147.f, 363.f }, 10.f );
	quests.emplace_back(q8);

	auto q9 = q8->AddChildQuest("Barbarians No Mo'");
	q9->AddFightObjective(BarbarianCamp::Location::West);
	quests.emplace_back(q9);


	////=====================================================
	////NPC3
	auto q10 = q5->AddChildQuest("Why Don't You Just Axe");
	q10->AddCollectObjective(Item::Type::Axe, 1);
	quests.emplace_back(q10);

	auto q11 = q10->AddChildQuest("Hungry For Apples");
	q11->AddCollectObjective(Item::Type::Food, 5);
	quests.emplace_back(q11);

	auto q12 = q11->AddChildQuest("Bye Bye Barbarians");
	q12->AddFightObjective(BarbarianCamp::Location::South);
	quests.emplace_back(q12);


	//auto q5 = q3->AddChildQuest("FIRST QUEST FOR NPC3");
	//q5->AddLocationObjective({ 38.0f, 20.3f, -574.5f }, 10);
	//quests.emplace_back(q5);

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

	quest->TriggerOnCompleteFunction();
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

Objective* QuestLog::GetTalkObjective(const std::string& NPC)
{
	for (auto quest : activeQuests)
	{
		auto objective = quest->GetTalkObjective(NPC);
		if (objective)
			return objective;
	}

	return nullptr;
}