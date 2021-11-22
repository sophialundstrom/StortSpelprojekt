#pragma once
#include "Objective.h"
#include "Target.h"

class TargetObjective : public Objective
{
	friend class Quest;
private:
	UINT targetID;
public:
	TargetObjective() = default;
	TargetObjective(UINT targetID);
	void Update(std::shared_ptr<Target> target);

	UINT GetTargetID() { return targetID; }

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};