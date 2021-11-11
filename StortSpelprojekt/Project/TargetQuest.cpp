#include "TargetQuest.h"

TargetQuest::TargetQuest(const std::string& name, bool active, bool completed, std::shared_ptr<Target> target)
	:Quest(name, Quest::Type::Target, active, completed), target(target) {}

void TargetQuest::Verify()
{
	if (target->GotHit())
		completed = true;
}

void TargetQuest::SetString(std::string& string)
{
	string = "Shoot marked target.";
}