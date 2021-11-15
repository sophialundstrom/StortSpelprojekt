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