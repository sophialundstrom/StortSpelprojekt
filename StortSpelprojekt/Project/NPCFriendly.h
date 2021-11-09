#pragma once
#include "NPCBase.h"
#include "Model.h"
#include "Building.h"
#include "QuestLog.h"
#include "Event.h"
#include "QuestMarker.h"


class FriendlyNPC : public NPC
{
public:
	FriendlyNPC(const std::string& file);
	FriendlyNPC(const Model& model);

	void AddQuestID(UINT ID) { questIDs.emplace_back(ID); }
	bool Interactable() { return interactable; }
	int GetQuestID() { return activeQuestID; }
	void BindBuilding(std::shared_ptr<Building> building) { this->building = building; }
	std::shared_ptr<Building> ConnectedBuilding() { return building; }
	std::shared_ptr<QuestMarker> GetQuestMarker() { return questMarker; }
	UINT NumConnectedQuests() { return questIDs.size(); }
	bool Completed() { return completed; }
	virtual void Update() override;

private:
	std::shared_ptr<QuestMarker> questMarker;
	std::shared_ptr<Building> building;
	int activeQuestID = -1;
	bool interactable = false;
	bool completed = false;
	std::vector<UINT> questIDs;
	void Walking();
	float timeMeasurement = 0;
	Timer timer;


	// Remove if we don't use pathing for these NPC's later
public:
	void SetPathVar(Pathfinding* path) { this->pathing = path; }
	void SetPlayerPtr(std::shared_ptr<Player>);
private:
	float speed = 9.0f;
	std::vector<Vector3> path;
	Pathfinding* pathing;
	std::shared_ptr<Player> player;
};