#pragma once
#include "Quest.h"

class CollectQuest : public Quest
{
private:
	unsigned int collectedItems = 0;
	unsigned int numItems;
public:
	CollectQuest() = default;

	// Inherited via Quest
	virtual void Activate() override
	{
		collectedItems = 0;
		numItems = 0;
		done = false;
	}

	virtual void Update() override
	{
		collectedItems++;
		if (collectedItems == numItems)
			Complete();
	}

	virtual void RenderUI() override
	{
		Print("", name);
	}
};