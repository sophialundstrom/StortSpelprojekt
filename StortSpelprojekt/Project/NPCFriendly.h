#pragma once
#include "NPCBase.h"
#include "Model.h"
#include "Building.h"
#include "QuestMarker.h"
#include "Quest.h"

class FriendlyNPC : public NPC
{
public:
	FriendlyNPC(const std::string& file);
	FriendlyNPC(const Model& model);

	void AddQuestID(UINT ID)								{ questIDs.emplace_back(ID); }
	bool Interactable()										{ return interactable; }
	int GetQuestID()										{ return activeQuestID; }
	void BindBuilding(std::shared_ptr<Building> building)	{ this->building = building; }
	std::shared_ptr<Building> ConnectedBuilding()			{ return building; }
	std::shared_ptr<QuestMarker> GetQuestMarker()			{ return questMarker; }
	UINT NumConnectedQuests()								{ return (UINT)questIDs.size(); }
	bool Completed()										{ return completed; }
	bool CompletedConversation()							{ return true; }

	virtual void Update() override;
private:
	std::vector<Quest*> quests;
	std::shared_ptr<QuestMarker> questMarker;
	std::shared_ptr<Building> building;
	int activeQuestID = -1;
	bool interactable = false;
	bool completed = false;

	std::vector<UINT> questIDs;
	void Walking();
};