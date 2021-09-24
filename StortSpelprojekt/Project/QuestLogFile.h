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
		auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".qsl";
		
		std::ifstream reader;
		
		reader.open(filePath, std::ios::beg);
		if (!reader.is_open())
		{
			Print("FAILED TO READ QUEST LOG FILE");
			return;
		}

		Quest* quest = nullptr;

		//TALK
		//TYPE ID ACTIVE COMPLETED NUMTRIGGER TRIGGERID(S) NAME NPCNAME

		//COLLECT
		//TYPE ID ACTIVE COMPLETED NUMTRIGGER TRIGGERID(S) NUMITEMS ITEMID COLLECTEDITEMS NAME

		//FIGHT
		//TYPE ID ACTIVE COMPLETED NUMTRIGGER TRIGGERID(S) NUMBARBS COMPLETEDBARBS NAME

		std::string line;
		while (!reader.eof())
		{
			UINT type, ID, numTriggerQuests;
			bool completed, active;

			reader >> type;
			reader >> ID;
			reader >> active;
			reader >> completed;

			if (completed)
			{
				std::getline(reader, line);
				continue;
			}
				
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

				UINT collectedItems;
				reader >> collectedItems;

				std::getline(reader, line);
				std::string name = GetNthString(line, 1);

				quest = new CollectQuest((QuestType)type, ID, name, active, numItems, itemID, collectedItems);
				break;
			}
				
			case QuestType::FIGHT:
			{
				UINT numTargets;
				reader >> numTargets;

				UINT completedTargets;
				reader >> completedTargets;

				std::getline(reader, line);
				std::string name = GetNthString(line, 1);

				quest = new FightQuest((QuestType)type, ID, name, active, numTargets, completedTargets);
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

	inline void Write(std::ofstream& writer, FightQuest* quest)
	{
		writer << quest->NumTargets() << " ";
		writer << quest->CompletedTargets();

		writer << " '";
		writer << quest->Name();
		writer << "' ";
	}

	inline void Write(std::ofstream& writer, CollectQuest* quest)
	{
		writer << quest->NumItems() << " ";
		writer << quest->ItemID() << " ";
		writer << quest->CollectedItems();

		writer << " '";
		writer << quest->Name();
		writer << "' ";
	}

	inline void Write(std::ofstream& writer, TalkQuest* quest)
	{
		writer << "'";
		writer << quest->Name();
		writer << "'";

		writer << " '";
		writer << quest->NPCName();
		writer << "' ";
	}

	inline void Save(const std::string& name, const std::map<UINT, Quest*>& quests)
	{
		auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + name + ".qsl";

		std::ofstream writer;

		writer.open(filePath, std::ios::trunc);
		if (!writer.is_open())
		{
			Print("FAILED TO CREATE QUEST LOG FILE");
			return;
		}

		for (auto& [ID, quest] : quests)
		{
			if (quest->IsCompleted())
				continue;

			writer << (UINT)quest->Type() << " ";
			writer << quest->GetID() << " ";
			writer << quest->IsActive() << " ";
			writer << quest->IsCompleted() << " ";
			
			const UINT numTriggerQuests = quest->GetTriggerQuests().size();

			writer << numTriggerQuests << " ";
			for (auto& questID : quest->GetTriggerQuests())
				writer << questID << " ";

			switch (quest->Type())
			{
			case QuestType::COLLECT:
				Write(writer, (CollectQuest*)quest);
				break;

			case QuestType::FIGHT:
				Write(writer, (FightQuest*)quest);
				break;

			case QuestType::TALK:
				Write(writer, (TalkQuest*)quest);
				break;
			}

			writer << "\n";
		}
	}
}