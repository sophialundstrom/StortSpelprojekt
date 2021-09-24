#pragma once
#include "Quest.h"

class CollectQuest : public Quest
{
private:
	UINT collectedItems = 0;
	UINT numItems;
	UINT itemID;
public:
	CollectQuest(QuestType type, UINT ID, const std::string& name, bool active, UINT numItems, UINT itemID, UINT collectedItems)
		:Quest(type, ID, name, active), numItems(numItems), itemID(itemID), collectedItems(collectedItems) {}

	UINT CollectedItems() { return collectedItems; }
	UINT NumItems() { return numItems; }
	UINT ItemID() { return itemID; }

	// Inherited via Quest
	virtual void Activate(std::shared_ptr<Player> player) override
	{
		Update(player);
	}

	virtual void Update(std::shared_ptr<Player> player) override
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