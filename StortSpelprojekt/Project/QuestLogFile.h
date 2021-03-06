#pragma once
#include "FileSystem.h"
#include "CollectQuest.h"
#include "FightQuest.h"
#include "TalkQuest.h"
#include <fstream>
#include <map>

namespace QuestLogFile
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
		//auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".qsl";
		//
		//std::ifstream reader;
		//
		//reader.open(filePath, std::ios::beg);
		//if (!reader.is_open())
		//{
		//	Print("FAILED TO READ QUEST LOG FILE");
		//	return;
		//}

		//Quest* quest = nullptr;

		//std::string line;
		//while (!reader.eof())
		//{
		//	UINT type, ID, numTriggerQuests;
		//	bool active;

		//	reader >> type;
		//	reader >> ID;
		//	reader >> active;
		//	reader >> numTriggerQuests;

		//	UINT* triggerQuests = new UINT[numTriggerQuests];
		//	for (UINT i = 0; i < numTriggerQuests; ++i)
		//		reader >> triggerQuests[i];

		//	/*switch ((QuestType)type)
		//	{

		//	case QuestType::TALK:
		//	{
		//		std::getline(reader, line);
		//		std::string name = GetNthString(line, 1);
		//		std::string NPC = GetNthString(line, 2);
		//		quest = new TalkQuest((QuestType)type, ID, name, active, NPC);
		//		break;
		//	}*/

		//	//case QuestType::COLLECT:
		//	//{
		//	//	UINT numItems;
		//	//	reader >> numItems;

		//	//	UINT itemID;
		//	//	reader >> itemID;

		//	//	std::getline(reader, line);
		//	//	std::string name = GetNthString(line, 1);

		//	//	//quest = new CollectQuest((QuestType)type, ID, name, active, numItems, (enum RESOURCE)itemID);
		//	//	break;
		//	//}
		//	/*	
		//	case QuestType::FIGHT:
		//	{
		//		UINT numTargets;
		//		reader >> numTargets;

		//		std::getline(reader, line);
		//		std::string name = GetNthString(line, 1);

		//		quest = new FightQuest((QuestType)type, ID, name, active, numTargets);
		//		break;
		//	}*/
		//	//}

		//	//for (UINT i = 0; i < numTriggerQuests; ++i)
		//	//	quest->AddTriggerQuest(triggerQuests[i]);

		//	//if (triggerQuests)
		//	//	delete[] triggerQuests;

		//	//quests.emplace(ID, quest);
		//}

		//reader.close();
		//Print("SUCCEEDED LOADING QUEST LOG");
	}

	inline void Save(const std::string& file, const std::map<UINT, Quest*>& quests)
	{
		auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".qsl";

		std::ofstream writer;

		writer.open(filePath, std::ios::beg);
		if (!writer.is_open())
		{
			Print("FAILED TO WRITE QUEST LOG FILE");
			return;
		}

		const std::string space = " ";
		for (auto& [ID, quest] : quests)
		{
			if (quest->IsCompleted())
				continue;

			//writer << (UINT)quest->Type() << space;
			writer << ID << space;
			writer << quest->IsActive() << space;

			//UINT numTriggerQuests = (UINT)quest->GetTriggerQuests().size();
			/*writer << numTriggerQuests << space;

			for (UINT i = 0; i < numTriggerQuests; ++i)
				writer << quest->GetTriggerQuests()[i] << space;*/

			//switch (quest->Type())
			//{
			//	case QuestType::TALK:
			//	{
			//		auto talkQuest = dynamic_cast<TalkQuest*>(quest);
			//		writer << "'" << talkQuest->Name() << "'" << space;
			//		writer << "'" << talkQuest->NPCName() << "'\n";
			//		break;
			//	}
			//	case QuestType::COLLECT:
			//	{
			//		/*auto collectQuest = dynamic_cast<CollectQuest*>(quest);
			//		writer << collectQuest->GetTargetAmount() << space;
			//		writer << (UINT)collectQuest->GetItemID() << space;
			//		writer << "'" << collectQuest->Name() << "'\n";*/
			//		break;
			//	}
			//	case QuestType::FIGHT:
			//	{
			//		auto fightQuest = dynamic_cast<FightQuest*>(quest);
			//		writer << fightQuest->NumTargets() << space;
			//		writer << "'" << fightQuest->Name() << "'\n";
			//		break;
			//	}
			//}
		}

		writer.close();
		Print("SUCCEEDED SAVING QUEST LOG");
	}
}