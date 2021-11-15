#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"

enum CombatStyle
{
	consistantDelay,
	Burst,
	wideArrow,
	Defenseless
};

class HostileNPC : public NPC
{
private:
	std::vector<std::shared_ptr<Arrow>>arrows;
	std::shared_ptr<Player> player;
	float movementXRadiant;
	float movementYRadiant;

	CombatStyle combatStyle;
	float enemyShootDetectionRadius = 150;
	float shootDeelay = 0.2f;

	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

	void Shoot(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, Vector3 startPos, Vector3 rotation);
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle);
	HostileNPC(const Model& model);
	void SwapCombatStyle(CombatStyle newCombatStyle);
	virtual void Update() override;
	void Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const std::shared_ptr<Player> player);
	bool CheckArrowHit(std::shared_ptr<Collider> collider, bool isDynamic = false);
private:
	void WeaponSlash();
};