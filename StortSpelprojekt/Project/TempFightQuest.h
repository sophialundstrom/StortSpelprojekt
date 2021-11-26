#pragma once
#include "TempQuest.h"
#include "BarbarianCamp.h"

class FightQuest : public Quest
{
private:
	UINT numCompleted;
	UINT numTargets;
	UINT campID;
public:
	FightQuest(const std::string& name, bool active, bool completed, UINT campID);

	void Verify(BarbarianCamp& camp);

	// Inherited via Quest
	virtual void SetString(std::string& string) override;
};