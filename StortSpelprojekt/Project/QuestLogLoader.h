#pragma once
#include "FileSystem.h"
#include "CollectQuest.h"
#include "FightQuest.h"
#include "TalkQuest.h"
#include <fstream>
#include <map>

namespace QuestLogLoader
{
	inline std::string GetNthString(const std::string& line, UINT n)
	{
		std::string temp = line;
		for (UINT i = 0; i < (n * 2 - 1); ++i)
		{
			size_t location = temp.find_first_of(39); // " ' " ascii
			temp = temp.substr(location + 1, temp.length());
		}

		return temp.substr(0, temp.find_first_of(39));
	}

	inline void Load(const std::string& file, std::map<UINT, Quest*>& quests)
	{
		auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".qsl";
		
		std::ifstream reader;
		
		reader.open(filePath, std::ios::beg);
		if (!reader.is_open())
		{
			Print("FAILED TO READ QUEST LOG FILE");
			return;
		}

		Quest* quest = nullptr;

		std::string line;
		while (!reader.eof())
		{
			UINT type, ID, numTriggerQuests;
			bool active;

			reader >> type;
			reader >> ID;
			reader >> active;
			reader >> numTriggerQuests;

			UINT* triggerQuests = new UINT[numTriggerQuests];
			for (UINT i = 0; i < numTriggerQuests; ++i)
				reader >> triggerQuests[i];

			switch ((QuestType)type)
			{

			case QuestType::TALK:
			{
				std::getline(reader, line);
				std::string name = GetNthString(line, 1);
				std::string NPC = GetNthString(line, 2);
				quest = new TalkQuest((QuestType)type, ID, name, active, NPC);
				break;
			}

			case QuestType::COLLECT:
			{
				UINT numItems;
				reader >> numItems;

				UINT itemID;
				reader >> itemID;

				std::getline(reader, line);
				std::string name = GetNthString(line, 1);

				quest = new CollectQuest((QuestType)type, ID, name, active, numItems, itemID);
				break;
			}
				
			case QuestType::FIGHT:
			{
				UINT numTargets;
				reader >> numTargets;

				std::getline(reader, line);
				std::string name = GetNthString(line, 1);

				quest = new FightQuest((QuestType)type, ID, name, active, numTargets);
				break;
			}
			}

			for (UINT i = 0; i < numTriggerQuests; ++i)
				quest->AddTriggerQuest(triggerQuests[i]);

			if (triggerQuests)
				delete[] triggerQuests;

			quests.emplace(ID, quest);
		}
	}
}
