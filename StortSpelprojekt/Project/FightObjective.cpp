#include "FightObjective.h"

FightObjective::FightObjective(BarbarianCamp::Location location)
	:Objective(Type::FIGHT, false), location(location) {}

void FightObjective::Update(BarbarianCamp* camp)
{
	cleared = camp->NumDead();
	total = camp->NumBarbarians();

	if (cleared == total)
		Complete();
}

std::string FightObjective::Info()
{
	if (BarbarianCamp::Locations[UINT(location)] != "Village")
		return "Clear The Baribarians At The " + BarbarianCamp::Locations[UINT(location)] + " Camp. " + std::to_string(cleared) + "/" + std::to_string(total) + ".";
	else
		return "Fight The Barbarians Attacking The Village! " + std::to_string(cleared) + "/" + std::to_string(total) + ".";;
}

void FightObjective::WriteToFile(File& file)
{
	Objective::WriteToFile(file);

	file.Write(location);
}

void FightObjective::ReadFromFile(File& file)
{
	this->type = Objective::Type::FIGHT;

	Objective::ReadFromFile(file);

	file.Read(location);
}