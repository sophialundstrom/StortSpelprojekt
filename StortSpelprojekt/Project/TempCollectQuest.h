//#pragma once
//#include "TempQuest.h"
//#include "Inventory.h"
//#include "Item.h"
//
//class CollectQuest : public Quest
//{
//private:
//	UINT collected;
//	UINT numToCollect;
//	Item::Type item;
//public:
//	CollectQuest(const std::string& name, bool active, bool completed, Item::Type item, UINT numToCollect);
//	void Verify(Inventory& inventory);
//	
//	// Inherited via Quest
//	virtual void SetString(std::string& string) override;
//};