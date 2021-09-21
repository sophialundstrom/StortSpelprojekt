#pragma once
#include "Quest.h"

class TalkQuest : public Quest
{
private:
	std::string NPC;
public:
	TalkQuest(QuestType type, UINT ID, const std::string& name, bool active, const std::string& NPC)
		:Quest(type, ID, name, active), NPC(NPC)
	{}

	// Inherited via Quest
	virtual void Activate(Player* player) override
	{

	}

	virtual void Update(Player* player) override
	{
		Complete();
	}

	virtual void RenderUI() override
	{
		Print(name);
	}
};