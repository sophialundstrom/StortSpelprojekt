#pragma once
#include "Objective.h"
#include "BarbarianCamp.h"

class FightObjective : public Objective
{
	friend class Quest;
private:
	UINT cleared;
	UINT total;
	BarbarianCamp::Location location;
public:
	FightObjective() = default;
	FightObjective(BarbarianCamp::Location location);
	void Update(BarbarianCamp* camp);

	BarbarianCamp::Location CampLocation() { return location; }

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};