#include "NPCFriendly.h"

FriendlyNPC::FriendlyNPC(const std::string& file)
	: NPC(file)
{
}

FriendlyNPC::FriendlyNPC(const Model& model)
	: NPC(model)
{
}

void FriendlyNPC::Update()
{
	NPC::Update();

	activeQuestID = -1;
	interactable = false;

	UINT completedQuests = 0;
	for (auto& ID : questIDs)
	{
		if (QuestLog::Inst().QuestIsActive(ID))
		{
			activeQuestID = ID;
			interactable = true;
			return;
		}

		if (QuestLog::Inst().QuestIsDone(ID))
			completedQuests++;
	}

	if (completedQuests >= questIDs.size())
		completed = true;
}

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
}