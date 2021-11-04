#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"

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
	std::vector<std::shared_ptr<Arrow>> playerArrows;
	std::shared_ptr<Player> player;
	float movementXRadiant;
	float movementYRadiant;

	CombatStyle combatStyle;
	float enemyShootDetectionRadius = 150;
	float shootDeelay = 0.2f;

	float shootDeelayPattern[3];
	int shootPatternIndex = 0;
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle);
	HostileNPC(const Model& model);
	void BindPlayerArrows(std::vector<std::shared_ptr<Arrow>> playerArrows);
	void BindArrows(ModelRenderer& modelrenderer);
	void SwapCombatStyle(CombatStyle newCombatStyle);
	virtual void Update() override;

private:
	void WeaponSlash();
};