#pragma once
#include "NPCBase.h"

class HostileNPC : public NPC
{
private:
	std::vector<std::shared_ptr<Arrow>> arrows;
public:
	HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows);
	HostileNPC(const Model& model);

	virtual void Update() override;

private:
	void WeaponSlash();
};