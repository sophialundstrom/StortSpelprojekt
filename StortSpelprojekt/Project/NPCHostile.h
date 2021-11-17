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
	
	ArrowHandler arrowHandler;
	std::shared_ptr<Player> player;
	float movementXRadiant;
	float movementYRadiant;

	CombatStyle combatStyle;
	float enemyShootDetectionRadius = 150;
	float shootDeelay = 0.2f;

	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

	void Shoot(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, Vector3 startPos, Vector3 rotation);

	ModelRenderer* mRend = nullptr;
	ColliderRenderer* cRend = nullptr;
	float lastShot = 0.f;
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, ModelRenderer& mRenderer, ColliderRenderer& cRenderer);
	HostileNPC(const Model& model);

	void SwapCombatStyle(CombatStyle newCombatStyle);
	virtual void Update() override;
	void Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const std::shared_ptr<Player> player);
	ArrowHandler GetArrowHandler() { return this->arrowHandler; }
	void CheckPlayerCollision(std::shared_ptr<Player> player);
	//bool CheckArrowHit(std::shared_ptr<Collider> collider, bool isDynamic = false);
private:
	void WeaponSlash();
};