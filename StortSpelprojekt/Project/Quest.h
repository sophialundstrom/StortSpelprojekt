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

	UINT GetID()
	{
		return ID;
	}

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
		Print(name);
		completed = true;
	}

	bool IsCompleted() { return completed; }

	virtual void Activate(Player* player) = 0;
	virtual void Update(Player* player) = 0;
	virtual void RenderUI() = 0;
};