#pragma once
#include "Quest.h"

class FightQuest : public Quest
{
private:
	unsigned int completedTargets = 0;
	unsigned int numTargets;
public:
	FightQuest(QuestType type, UINT ID, const std::string& name, bool active, UINT numTargets)
		:Quest(type, ID, name, active), numTargets(numTargets)
	{}

	UINT NumTargets() { return numTargets; }
	UINT CompletedTargets() { return completedTargets; }

	// Inherited via Quest
	virtual void Activate(std::shared_ptr<Player> player) override
	{
		active = true;
		completedTargets = player->Stats().barbariansKilled;
		numTargets += completedTargets;
	}

	virtual void Update(std::shared_ptr<Player> player) override
	{
		completedTargets = player->Stats().barbariansKilled;
		if (completedTargets == numTargets)
			Complete();
	}

	virtual void UpdateUI(std::string& string) override
	{
		string = "Barbarians left: " + std::to_string(numTargets - completedTargets) + "/" + std::to_string(numTargets);
	}
};