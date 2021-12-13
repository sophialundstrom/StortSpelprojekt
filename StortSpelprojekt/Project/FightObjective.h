#pragma once
#include "Objective.h"
#include "BarbarianCamp.h"

class FightObjective : public Objective
{
	friend class Quest;
private:
	UINT cleared;
	UINT total;
	CampData::Location location;
public:
	FightObjective() = default;
	FightObjective(CampData::Location location);
	void Update(BarbarianCamp* camp);

	CampData::Location CampLocation() { return location; }

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};