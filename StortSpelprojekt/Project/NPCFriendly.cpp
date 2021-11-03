#include "NPCFriendly.h"
#include "QuestLog.h"


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

	for (auto& ID : questIDs)
	{
		if (QuestLog::Inst().QuestIsActive(ID))
		{

		}
	}
}

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
}