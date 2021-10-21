#pragma once
#include "NPCBase.h"

class HostileNPC : public NPC
{
private:
	std::vector<std::shared_ptr<Arrow>> arrows;
	std::shared_ptr<Player> player;
	float movementXRadiant;
	float movementYRadiant;
public:
	HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player);
	HostileNPC(const Model& model);

	virtual void Update() override;

private:
	void WeaponSlash();
};