#include "TempFightQuest.h"

FightQuest::FightQuest(const std::string& name, bool active, bool completed, UINT campID)
	:Quest(name, Quest::Type::Fight, active, completed), 
	numTargets(BarbarianCamp::NUM_BARBARIANS), campID(campID), numCompleted(0) {}

void FightQuest::Verify(BarbarianCamp& camp)
{
	numCompleted = camp.NumDead();

	if (numCompleted >= numTargets)
		completed = true;
}

void FightQuest::SetString(std::string& string)
{
	string = "Barbarians Slain: " + std::to_string(numCompleted) + " / " + std::to_string(numTargets);
}