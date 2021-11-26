//#include "TempCollectQuest.h"
//
//CollectQuest::CollectQuest(const std::string& name, bool active, bool completed, Item::Type item, UINT numToCollect)
//	:Quest(name, Quest::Type::Collect, active, completed), item(item), numToCollect(numToCollect) {}
//
//void CollectQuest::Verify(Inventory& inventory)
//{
//	collected = inventory.NumOf(item);
//
//	if (collected >= numToCollect)
//		completed = true;
//}
//
//void CollectQuest::SetString(std::string& string)
//{
//	if (numToCollect > 1)
//		string = Item::Names[(UINT)type] + "Collected: " + std::to_string(collected) + " / " + std::to_string(numToCollect);
//	else
//		string = "Collect " + Item::Names[(UINT)type];
//}