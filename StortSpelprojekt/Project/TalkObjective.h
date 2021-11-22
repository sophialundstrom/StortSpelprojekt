#pragma once
#include "Objective.h"
#include "NPCFriendly.h"

class TalkObjective : public Objective
{
	friend class Quest;
private:
	std::string NPC;
	std::string string;
public:
	TalkObjective() = default;
	TalkObjective(const std::string& NPC, const std::string& string);

	std::string GetNPCName() { return NPC; }
	std::string GetString()	 { return string; }

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};