#pragma once
#include "Quest.h"

class TalkQuest : public Quest
{
private:
	unsigned int npcID;
public:
	TalkQuest(const std::string& name, unsigned int ID, unsigned int npcID)
		:Quest(name, ID), npcID(npcID) {}

	// Inherited via Quest
	virtual void Update() override
	{
		Complete();
	}

	virtual void RenderUI() override
	{
		Print("", name);
	}
};