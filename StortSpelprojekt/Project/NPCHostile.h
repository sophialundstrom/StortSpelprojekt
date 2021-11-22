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
private:
	void WeaponSlash();


	// Remove if we don't use pathing for these NPC's later
public:
	void SetPathVar(Pathfinding* path)				{ this->pathing = path; }
	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
private:
	float speed = 9.0f;
	std::vector<Vector3> path;
	Pathfinding* pathing;
	std::shared_ptr<Player> player;
};