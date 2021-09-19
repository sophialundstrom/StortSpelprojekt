#pragma once
#include "Quest.h"

class CollectQuest : public Quest
{
private:
	unsigned int collectedItems = 0;
	unsigned int numItems;
public:
	CollectQuest(QuestType type, UINT ID, const std::string& name, bool active)
		:Quest(type, ID, name, active)
	{

	}

	// Inherited via Quest
	virtual void Activate() override
	{
		collectedItems = 0;
		numItems = 0;
		completed = false;
	}

	virtual void Update(Player* player) override
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