#pragma once
#include "Quest.h"

class TalkQuest : public Quest
{
private:

public:
	TalkQuest(QuestType type, UINT ID, const std::string& name, bool active)
		:Quest(type, ID, name, active)
	{

	}

	// Inherited via Quest
	virtual void Activate() override
	{

	}

	virtual void Update(Player* player) override
	{
		Complete();
	}

	virtual void RenderUI() override
	{
		Print("", name);
	}
};