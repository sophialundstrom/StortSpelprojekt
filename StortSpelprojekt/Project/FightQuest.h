#pragma once
#include "Quest.h"

class FightQuest : public Quest
{
private:
	unsigned int completedTargets;
	unsigned int numTargets;
public:
	FightQuest(QuestType type, UINT ID, const std::string& name, bool active, UINT numTargets)
		:Quest(type, ID, name, active), numTargets(numTargets), completedTargets(0)
	{}

	// Inherited via Quest
	virtual void Activate(Player* player) override
	{
		completedTargets = player->GameStats().barbariansKilled;
		numTargets += completedTargets;
	}

	virtual void Update(Player* player) override
	{
		completedTargets = player->GameStats().barbariansKilled;
		if (completedTargets == numTargets)
			Complete();
	}

	virtual void RenderUI() override
	{
		Print(name);
	}
};