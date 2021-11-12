#include "TargetObjective.h"

TargetObjective::TargetObjective(bool completed, std::shared_ptr<Target> target)
	:Objective(Type::TARGET, completed), target(target) {}

void TargetObjective::Update()
{
	if (target->GotHit())
		completed = true;
}

std::string TargetObjective::Info()
{
	return "Shoot The Marked Target.";
}