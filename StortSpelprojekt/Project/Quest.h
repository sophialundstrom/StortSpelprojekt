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
	bool active;
	bool completed;
	std::vector<Objective*> objectives;
	std::vector<Quest*> childQuests;
public:
	Quest() = default;
	Quest(const std::string& name, bool active, bool completed);

	void Activate()							{ active = true; }
	bool IsCompleted()						{ return completed; }
	bool IsActive()							{ return active; }
	std::vector<Quest*>& GetChildQuests()	{ return childQuests; }

	void Update(std::shared_ptr<Player> player, std::vector<BarbarianCamp> camps, std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs, std::vector<std::shared_ptr<Target>> targets);

	void SaveToFile(File& file);
	void LoadFromFile(File& file);
};