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
}

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
}