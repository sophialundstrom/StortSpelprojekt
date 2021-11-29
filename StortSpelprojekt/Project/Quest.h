#pragma once
#include <memory>
#include <vector>
#include "Player.h"

enum class QuestType { TALK, COLLECT, FIGHT };

class Quest
{
private:
	QuestType type;
	UINT ID;
protected:
	std::string name;
	bool active = false;
	bool completed = false;
	std::vector<UINT> triggerQuests;
public:
	Quest(QuestType type, UINT ID, const std::string& name, bool active)
		:type(type), ID(ID), name(name), active(active)
	{}

	UINT GetID() { return ID; }

	std::string Name() { return name; }

	QuestType Type() { return type; }

	void AddTriggerQuest(UINT ID)
	{
		triggerQuests.emplace_back(ID);
	}

	const std::vector<UINT>& GetTriggerQuests()
	{
		return triggerQuests;
	}

	void Complete()
	{
		Print(name, "Completed Quest");
		completed = true;
	}

	bool IsCompleted() { return completed; }
	bool IsActive() { return active; }

	virtual void Activate(std::shared_ptr<Player> player) = 0;
	virtual void Update(std::shared_ptr<Player> player) = 0;
	virtual void UpdateUI(std::string& string) = 0;
};