#pragma once
#include "Objective.h"
#include "NPCFriendly.h"

class TalkObjective : public Objective
{
private:
	std::string NPC;
public:
	TalkObjective() = default;
	TalkObjective(const std::string& NPC);
	void Update(std::shared_ptr<FriendlyNPC> NPC);

	std::string GetNPCName() { return NPC; }

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};