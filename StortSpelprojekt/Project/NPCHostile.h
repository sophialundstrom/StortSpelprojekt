#pragma once
#include "NPCBase.h"

class HostileNPC : public NPC
{
public:
	HostileNPC(const std::string& file);
	HostileNPC(const Model& model);

	virtual void Update() override;

private:
	void WeaponSlash();
};