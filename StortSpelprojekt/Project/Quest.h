#pragma once
#include <string>
#include <memory>
#include <vector>

class Quest
{
private:
	bool active = false;
	bool done = false;

	unsigned int ID = -1;
	
	unsigned int numTriggerQuests = 0;
	std::vector<std::shared_ptr<Quest>> triggerQuests;
public:
	Quest(unsigned int ID)
		:ID(ID) {}

	void AddTriggerQuest(std::shared_ptr<Quest> quest)
	{
		triggerQuests.emplace_back(quest);
		numTriggerQuests++;
	}

	void Complete()
	{
		active = false;
		done = true;
		for (auto& quest : triggerQuests)
			quest->Activate();
	}

	void Activate()
	{
		active = true;
	}

	bool IsActive() { return active; }
};