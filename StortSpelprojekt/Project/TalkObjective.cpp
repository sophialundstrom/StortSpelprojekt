#include "TalkObjective.h"

TalkObjective::TalkObjective(const std::string& NPC)
	:Objective(Type::TALK, false), NPC(NPC) {}

void TalkObjective::Update(std::shared_ptr<FriendlyNPC> NPC)
{
	if (NPC->CompletedConversation())
		completed = true;
}

std::string TalkObjective::Info()
{
	return "Talk To " + NPC + ".";
}

void TalkObjective::WriteToFile(File& file)
{
	Objective::WriteToFile(file);

	file.WriteString(NPC.c_str());
}

void TalkObjective::ReadFromFile(File& file)
{
	this->type = Objective::Type::TALK;

	Objective::ReadFromFile(file);

	//IF LUCKY WE MIGHT BE ABLE TO USE THIS->NPC = READSTRING AND STRING DELETE IN DESTRUCTOR FOR US
	char* string = file.ReadString();

	this->NPC = string;

	delete[] string;
}