#include "Quest.h"

#include "NPCFriendly.h"
#include "CollectObjective.h"
#include "FightObjective.h"
#include "TargetObjective.h"
#include "TalkObjective.h"
#include "LocationObjective.h"
#include "InGameOverlay.h"

Quest::Quest(const std::string& name, bool active, bool completed, bool unlocked)
	:name(name), active(active), completed(completed), unlocked(unlocked) {}

Quest* Quest::AddChildQuest(const std::string& name)
{
	auto quest = new Quest(name, false, false);
	childQuests.emplace_back(quest);
	return quest;
}

void Quest::AddCollectObjective(Item::Type type, UINT amount)
{
	objectives.emplace_back(new CollectObjective(type, amount));
}

void Quest::AddTalkObjective(const std::string& NPC, const std::string& string)
{
	objectives.emplace_back(new TalkObjective(NPC, string));
}

void Quest::AddTargetObjective(UINT targetID)
{
	objectives.emplace_back(new TargetObjective(targetID));
}

void Quest::AddFightObjective(BarbarianCamp::Location location)
{
	objectives.emplace_back(new FightObjective(location));
}

void Quest::AddLocationObjective(const Vector3& location, float radius)
{
	objectives.emplace_back(new LocationObjective(location, radius));
}

void Quest::Update(std::shared_ptr<Player> player, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets)
{
	UINT numCompleted = 0;

	for (auto& objective : objectives)
	{
		if (objective->IsCompleted())
		{
			numCompleted++;
			continue;
		}

		auto collect = dynamic_cast<CollectObjective*>(objective);
		if (collect)
		{
			collect->Update(player->Inventory());

			if (collect->IsCompleted())
				numCompleted++;

			continue;
		}

		auto fight = dynamic_cast<FightObjective*>(objective);
		if (fight)
		{
			fight->Update(camps[fight->CampLocation()]);

			if (fight->IsCompleted())
				numCompleted++;

			continue;
		}

		auto talk = dynamic_cast<TalkObjective*>(objective);
		if (talk)
		{
			if (talk->IsCompleted())
				numCompleted++;

			continue;
		}

		auto target = dynamic_cast<TargetObjective*>(objective);
		if (target)
		{
			for (auto& t : targets)
			{
				if (t->GetID() == target->GetTargetID())
				{
					target->Update(t);
					break;
				}
			}

			if (target->IsCompleted())
				numCompleted++;

			continue;
		}

		auto location = dynamic_cast<LocationObjective*>(objective);
		if (location)
		{
			location->Update(player->GetPosition());

			if (location->IsCompleted())
				numCompleted++;

			continue;
		}
	}

	if (numCompleted == objectives.size())
	{
		if (!waiting)
		{
			waiting = true;
			std::thread thread([=]
				{
					Sleep(2000);
					completed.store(true);
					//completed = true;
				});

			thread.detach();
		}
	}
}

void Quest::SaveToFile(File& file)
{
	file.WriteString(name.c_str());
	file.Write(unlocked);
	file.Write(active);
	file.Write((bool)completed);

	file.Write((UINT)objectives.size());

	for (auto& objective : objectives)
	{
		auto collect = dynamic_cast<CollectObjective*>(objective);
		if (collect)
		{
			collect->WriteToFile(file);
			continue;
		}

		auto fight = dynamic_cast<FightObjective*>(objective);
		if (fight)
		{
			fight->WriteToFile(file);
			continue;
		}

		auto talk = dynamic_cast<TalkObjective*>(objective);
		if (talk)
		{
			talk->WriteToFile(file);
			continue;
		}

		auto target = dynamic_cast<TargetObjective*>(objective);
		if (target)
		{
			target->WriteToFile(file);
			continue;
		}

		auto location = dynamic_cast<LocationObjective*>(objective);
		if (location)
		{
			location->WriteToFile(file);
			continue;
		}
	}

	file.Write(childQuests.size());
	for (auto child : childQuests)
		child->SaveToFile(file);
}

void Quest::LoadFromFile(File& file)
{
	char* string = file.ReadString();
	name = string;
	delete[] string;

	file.Read(unlocked);
	file.Read(active);
	file.Read(completed);

	UINT numObjectives;
	file.Read(numObjectives);

	for (UINT i = 0; i < numObjectives; ++i)
	{
		Objective::Type type;
		file.Read(type);

		Objective* objective;

		switch (type)
		{
			case Objective::Type::COLLECT:
			{
				objective = new CollectObjective();
				objective->ReadFromFile(file);
				break;
			}

			case Objective::Type::FIGHT:
			{
				objective = new FightObjective();
				objective->ReadFromFile(file);
				break;
			}

			case Objective::Type::TALK:
			{
				objective = new TalkObjective();
				objective->ReadFromFile(file);
				break;
			}

			case Objective::Type::TARGET:
			{
				objective = new TargetObjective();
				objective->ReadFromFile(file);
				break;
			}

			case Objective::Type::LOCATION:
			{
				objective = new LocationObjective();
				objective->ReadFromFile(file);
				break;
			}
		}

		objectives.emplace_back(objective);
	}

	size_t numChildren;
	file.Read(numChildren);

	for (UINT i = 0; i < numChildren; ++i)
	{
		auto child = new Quest();
		child->LoadFromFile(file);
		childQuests.emplace_back(child);
	}
}

void Quest::ResetObjectiveResources(std::shared_ptr<Player> player, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets)
{
	for (auto& objective : objectives)
	{
		auto collect = dynamic_cast<CollectObjective*>(objective);
		if (collect)
		{
			player->Inventory().RemoveItem(collect->itemType, collect->numToCollect);
			continue;
		}

		auto fight = dynamic_cast<FightObjective*>(objective);
		if (fight)
		{
			camps[fight->CampLocation()]->Reset();
			continue;
		}

		auto target = dynamic_cast<TargetObjective*>(objective);
		if (target)
		{
			for (auto& t : targets)
			{
				if (target->GetTargetID() == t->GetID())
				{
					t->Reset();
					break;
				}
			}

			continue;
		}
	}
}

Objective* Quest::GetTalkObjective(const std::string& NPC)
{
	for (auto objective : objectives)
	{
		auto talk = dynamic_cast<TalkObjective*>(objective);
		if (talk && talk->GetNPCName() == NPC)
			return talk;
	}

	return nullptr;
}
