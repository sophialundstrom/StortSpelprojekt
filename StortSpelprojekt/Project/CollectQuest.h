//#pragma once
//#include "Quest.h"
//
//class CollectQuest : public Quest
//{
//private:
//	UINT collectedItems = 0;
//	UINT numItems;
//	RESOURCE itemID;
//public:
//	CollectQuest(QuestType type, UINT ID, const std::string& name, bool active, UINT numItems, enum RESOURCE itemID)
//		:Quest(type, ID, name, active), numItems(numItems), itemID(itemID)
//	{}
//
//	UINT GetTargetAmount() { return numItems; }
//	RESOURCE GetItemID() { return itemID; }
//
//	// Inherited via Quest
//	virtual void Activate(std::shared_ptr<Player> player) override
//	{
//		active = true;
//	}
//
//	virtual void Update(std::shared_ptr<Player> player) override
//	{
//		collectedItems = player->Inventory().NumOf(itemID);
//		if (collectedItems >= numItems)
//		{
//			player->Inventory().RemoveItem(itemID, numItems);
//			Complete();
//		}
//	}
//
//	virtual void UpdateUI(std::string& string) override
//	{
//		string = "Collected: " + std::to_string(collectedItems) + "/" + std::to_string(numItems);
//	}
//};