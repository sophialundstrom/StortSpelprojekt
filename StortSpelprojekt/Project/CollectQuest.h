#pragma once
#include "Quest.h"

class CollectQuest : public Quest
{
private:
	UINT collectedItems = 0;
	UINT numItems;
	UINT itemID;
public:
	CollectQuest(QuestType type, UINT ID, const std::string& name, bool active, UINT numItems, UINT itemID)
		:Quest(type, ID, name, active), numItems(numItems), itemID(itemID)
	{}

	// Inherited via Quest
	virtual void Activate(Player* player) override
	{
		Update(player);
	}

	virtual void Update(Player* player) override
	{
		collectedItems = player->Inventory().NumOf(itemID);
		if (collectedItems == numItems)
			Complete();
	}

	virtual void RenderUI() override
	{
		Print(name);
	}
};