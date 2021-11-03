#pragma once
#include "NPCBase.h"

enum CombatStyle
{
	consistantDelay,
	Burst,
	wideArrow
};

class HostileNPC : public NPC
{
private:
	std::vector<std::shared_ptr<Arrow>> arrows;
	std::shared_ptr<Player> player;
	float movementXRadiant;
	float movementYRadiant;

	CombatStyle combatStyle;
	float enemyShootDetectionRadius = 150.f;
	float shootDeelay = 0.2f;

	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

public:
	HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player, CombatStyle combatStyle);
	HostileNPC(const Model& model);
	void SwapCombatStyle(CombatStyle newCombatStyle);
	virtual void Update() override;

private:
	void WeaponSlash();
};