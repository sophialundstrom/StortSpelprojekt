#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"

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
	Vector3 spawnPosition;

	ArrowHandler arrowHandler;

	std::shared_ptr<Player> player;

	float movementXRadiant;
	float movementYRadiant;

	CombatStyle combatStyle;
	float enemyShootDetectionRadius = 150;
	float shootDeelay = 0.2f;

	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

	ModelRenderer* mRend = nullptr;
	ColliderRenderer* cRend = nullptr;
	float lastShot = 0.f;

	float detectionRadius = 10.0f;
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle);
	HostileNPC(const Model& model);

	float DetectionRadius() { return detectionRadius; }

	void SwapCombatStyle(CombatStyle newCombatStyle);
	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player);
	ArrowHandler GetArrowHandler() { return this->arrowHandler; }
	void SetSpawnPosition(const Vector3& position);
	void Reset();
	void CheckPlayerCollision(std::shared_ptr<Player> player);
private:
	void WeaponSlash();
};