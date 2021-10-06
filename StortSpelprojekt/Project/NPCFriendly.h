#pragma once
#include "NPCBase.h"
#include "Model.h"

class FriendlyNPC : public NPC
{
public:
	FriendlyNPC(const std::string& file);
	FriendlyNPC(const Model& model);

	virtual void Update() override;
private:


	void Walking();
};