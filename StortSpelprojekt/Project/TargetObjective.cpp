#include "TargetObjective.h"

TargetObjective::TargetObjective(UINT targetID)
	:Objective(Type::TARGET, false), targetID(targetID) {}

void TargetObjective::Update(std::shared_ptr<Target> target)
{
	if (target->GotHit())
		completed = true;
}

std::string TargetObjective::Info()
{
	return "Shoot The Marked Target.";
}

void TargetObjective::WriteToFile(File& file)
{
	Objective::WriteToFile(file);
	
	file.Write(targetID);
}

void TargetObjective::ReadFromFile(File& file)
{
	this->type = Objective::Type::TARGET;

	Objective::ReadFromFile(file);

	file.Read(targetID);
}