//#include "LocationQuest.h"
//
//LocationQuest::LocationQuest(const std::string& name, bool active, bool completed, float range, Vector3 location)
//	:Quest(name, Quest::Type::Location, active, completed), range(range), location(location) {}
//
//void LocationQuest::Verify(Vector3 playerPosition)
//{
//	if ((playerPosition - location).Length() < range)
//		completed = true;
//}
//
//void LocationQuest::SetString(std::string& string)
//{
//	string = "Travel to mapped location.";
//}