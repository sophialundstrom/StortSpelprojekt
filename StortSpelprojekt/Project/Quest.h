#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Objective.h"
#include "BarbarianCamp.h"
#include "Target.h"

class InGameOverlay;

class FriendlyNPC;

class Quest
{
protected:
	std::string questHolderNPC;
	std::string name;

	bool unlocked;
	bool active;
	std::atomic<bool> completed;
	//bool completed;
	bool waiting;

	std::vector<Objective*> objectives;
	std::vector<Quest*> childQuests;

	std::function<void()> onUnlockedFunction = NULL;
	std::function<void()> onActivateFunction = NULL;
	std::function<void()> onCompleteFunction = NULL;
public:
	Quest() = default;
	Quest(const std::string& name, bool active, bool completed, bool unlocked = false);

	const std::vector<Objective*> GetObjectives() { return objectives; }

	const std::string GetName()				{ return name; }

	void Unlock()		{ if (!unlocked) TriggerOnUnlockedFunction(); unlocked = true; }
	bool Unlocked()		{ return unlocked; }
	void Activate()		{ if (!active) TriggerOnActivateFunction(); active = true; }
	bool IsCompleted()	{ return completed.load(); }
	bool IsActive()		{ return active; }

	void SetQuestHolder(const std::string& NPC) { questHolderNPC = NPC; }
	const std::string GetQuestHolder() { return questHolderNPC; }

	std::vector<Quest*>& GetChildQuests()	{ return childQuests; }

	Quest* AddChildQuest(const std::string& name);

	void AddCollectObjective(Item::Type type, UINT amount);
	void AddTalkObjective(const std::string& NPC, const std::string& string);
	void AddTargetObjective(UINT targetID);
	void AddFightObjective(BarbarianCamp::Location location);
	void AddLocationObjective(const Vector3& location, float radius);

	void AddOnUnlockedFunction(std::function<void()> function) { onUnlockedFunction = function; }
	void AddOnActivateFunction(std::function<void()> function) { onActivateFunction = function; }
	void AddOnCompleteFunction(std::function<void()> function) { onCompleteFunction = function; }

	void TriggerOnUnlockedFunction() { if (onUnlockedFunction != NULL) onUnlockedFunction(); }
	void TriggerOnActivateFunction() { if (onActivateFunction != NULL) onActivateFunction(); }
	void TriggerOnCompleteFunction() { if (onCompleteFunction != NULL) onCompleteFunction(); }

	void ResetObjectiveResources(std::shared_ptr<Player>, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets);
	void Update(std::shared_ptr<Player> player, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets, std::vector<std::shared_ptr<FriendlyNPC>> frendlyNPCs);

	void SaveToFile(File& file);
	void LoadFromFile(File& file);

	Objective* GetTalkObjective(const std::string& NPC);
};