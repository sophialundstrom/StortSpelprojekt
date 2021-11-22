#include "TalkObjective.h"

TalkObjective::TalkObjective(const std::string& NPC, const std::string& string)
	:Objective(Type::TALK, false), NPC(NPC), string(string) {}

std::string TalkObjective::Info()
{
	return "Talk To " + NPC + ".";
}

void TalkObjective::WriteToFile(File& file)
{
	Objective::WriteToFile(file);

	file.WriteString(NPC.c_str());
	file.WriteString(string.c_str());
}

void TalkObjective::ReadFromFile(File& file)
{
	this->type = Objective::Type::TALK;

	Objective::ReadFromFile(file);

	//IF LUCKY WE MIGHT BE ABLE TO USE THIS->NPC = READSTRING AND STRING DELETE IN DESTRUCTOR FOR US
	char* string = file.ReadString();
	this->NPC = string;
	delete[] string;

	string = file.ReadString();
	this->string = string;
}