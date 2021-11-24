#include "TargetObjective.h"
#include "Renderers.h"

TargetObjective::TargetObjective(UINT targetID)
	:Objective(Type::TARGET, false), targetID(targetID) {}

void TargetObjective::Update(std::shared_ptr<Target> target)
{
	if (!IR->IsBound(target))
		IR->Bind(target);

	if (target->GotHit())
	{
		Complete();
		if (IR->IsBound(target))
			IR->Unbind(target);
	}
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