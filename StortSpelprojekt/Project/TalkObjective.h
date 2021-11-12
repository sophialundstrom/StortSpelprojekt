#pragma once
#include "Objective.h"
#include "NPCFriendly.h"

class TalkObjective : public Objective
{
private:
	std::shared_ptr<FriendlyNPC> NPC;
public:
	TalkObjective(bool completed, std::shared_ptr<FriendlyNPC> NPC);
	void Update();

	// Inherited via Objective
	virtual std::string Info() override;
};