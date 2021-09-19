#pragma once
#include "Quest.h"

class FightQuest : public Quest
{
private:
	unsigned int completedTargets = 0;
	unsigned int numTargets;
public:
	FightQuest(QuestType type, UINT ID, const std::string& name, bool active)
		:Quest(type, ID, name, active)
	{

	}

	// Inherited via Quest
	virtual void Activate() override
	{

	}

	virtual void Update(Player* player) override
	{
		completedTargets++;
		if (completedTargets == numTargets)
			Complete();
	}

	virtual void RenderUI() override
	{
		Print("", name);
	}
};