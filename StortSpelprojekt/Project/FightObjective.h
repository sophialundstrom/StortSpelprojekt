#pragma once
#include "Objective.h"
#include "BarbarianCamp.h"

class FightObjective : public Objective
{
private:
	BarbarianCamp::Location location;
public:
	FightObjective(bool completed, BarbarianCamp::Location location);
	void Update(BarbarianCamp& camp);

	BarbarianCamp::Location CampLocation() { return location; }

	// Inherited via Objective
	virtual std::string Info() override;
};