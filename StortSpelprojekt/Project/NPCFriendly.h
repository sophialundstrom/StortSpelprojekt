#pragma once
#include "NPCBase.h"
#include "Model.h"

class FriendlyNPC : public NPC
{
public:
	FriendlyNPC(const std::string& file);
	FriendlyNPC(const Model& model);

	void AddQuestID(UINT ID) { questIDs.emplace_back(ID); }

	virtual void Update() override;
private:
	std::vector<UINT> questIDs;
	void Walking();
};