#include "FightObjective.h"

FightObjective::FightObjective(bool completed, BarbarianCamp::Location location)
	:Objective(Type::FIGHT, completed), location(location) {}

void FightObjective::Update(BarbarianCamp& camp)
{
	if (camp.NumDead() == camp.NumBarbarians())
		completed = true;
}

std::string FightObjective::Info()
{
	return "Clear The Baribarians At The " + BarbarianCamp::Locations[UINT(location)] + " Camp.";
}