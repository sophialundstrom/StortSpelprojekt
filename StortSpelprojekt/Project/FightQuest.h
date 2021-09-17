#pragma once
#include "Quest.h"

class FightQuest : public Quest
{
private:
	unsigned int completedTargets = 0;
	unsigned int numTargets;
public:
	FightQuest(const std::string& name, unsigned int ID, unsigned int numTargets)
		:Quest(name, ID), numTargets(numTargets) {}

	// Inherited via Quest
	virtual void Update() override
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