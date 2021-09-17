#pragma once
#include "Quest.h"

class CollectQuest : public Quest
{
private:
	unsigned int collectedItems = 0;
	unsigned int numItems;
public:
	CollectQuest(unsigned int ID, unsigned int numItems)
		:Quest(ID), numItems(numItems) {}

	void AddProgress()
	{
		collectedItems++;
		if (collectedItems == numItems)
			Complete();
	}
};