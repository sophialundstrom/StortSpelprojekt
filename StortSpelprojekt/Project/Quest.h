#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Objective.h"
#include "BarbarianCamp.h"
#include "Target.h"

class FriendlyNPC;

class Quest
{
protected:
	std::string name;
	bool unlocked;
	bool active;
	bool completed;
	std::vector<Objective*> objectives;
	std::vector<Quest*> childQuests;
public:
	Quest() = default;
	Quest(const std::string& name, bool active, bool completed);

	const std::vector<Objective*> GetObjectives() { return objectives; }

	const std::string GetName()				{ return name; }

	void Unlock()		{ unlocked = true; }
	bool Unlocked()		{ return unlocked; }
	void Activate()		{ active = true; }
	bool IsCompleted()	{ return completed; }
	bool IsActive()		{ return active; }

	std::vector<Quest*>& GetChildQuests()	{ return childQuests; }

	Quest* AddChildQuest(const std::string& name);

	void AddCollectObjective(Item::Type type, UINT amount);
	void AddTalkObjective(const std::string& NPC);
	void AddTargetObjective(UINT targetID);
	void AddFightObjective(BarbarianCamp::Location location);
	void AddLocationObjective(const Vector3& location, float radius);

	void Update(std::shared_ptr<Player> player, std::vector<BarbarianCamp> camps, std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs, std::vector<std::shared_ptr<Target>> targets);

	void SaveToFile(File& file);
	void LoadFromFile(File& file);
};