#pragma once
#include "Quest.h"

class TalkQuest : public Quest
{
private:
	unsigned int npcID;
public:
	TalkQuest(unsigned int ID, unsigned int npcID)
		:Quest(ID), npcID(npcID) {}

	void AddProgress()
	{
		Complete();
	}
};