#pragma once
#include "Objective.h"
#include "Target.h"

class TargetObjective : public Objective
{
private:
	std::shared_ptr<Target> target;
public:
	TargetObjective(bool completed, std::shared_ptr<Target> target);
	void Update();

	// Inherited via Objective
	virtual std::string Info() override;
};