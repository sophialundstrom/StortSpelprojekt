#include "TalkObjective.h"

TalkObjective::TalkObjective(bool completed, std::shared_ptr<FriendlyNPC> NPC)
	:Objective(Type::TALK, completed), NPC(NPC) {}

void TalkObjective::Update()
{
	if (NPC->CompletedConversation())
		completed = true;
}

std::string TalkObjective::Info()
{
	return "Talk To " + NPC->GetName() + ".";
}