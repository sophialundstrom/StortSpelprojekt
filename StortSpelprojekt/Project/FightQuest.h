#pragma once
#include "Quest.h"

class FightQuest : public Quest
{
private:
	unsigned int completedTargets = 0;
	unsigned int numTargets;
public:
	FightQuest(unsigned int ID, unsigned int numTargets)
		:Quest(ID), numTargets(numTargets) {}

	void AddProgress()
	{
		completedTargets++;
		if (completedTargets == numTargets)
			Complete();
	}
};