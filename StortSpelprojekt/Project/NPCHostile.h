#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;

class HostileNPC : public NPC
{
private:
	ArrowHandler arrowHandler;
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle);
	HostileNPC(const Model& model);
;
	virtual void Update() override;
	ArrowHandler GetArrowHandler() { return this->arrowHandler; }
	void SetSpawnPosition(const Vector3& position);
	void Reset();
	void CheckPlayerCollision(std::shared_ptr<Player> player);
	std::shared_ptr<Player> GetPlayer() { return this->player; }
private:
	void WeaponSlash();


	// Remove if we don't use pathing for these NPC's later
public:
	void SetPathVar(Pathfinding* path)				{ this->pathing = path; }
	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetState(NPCState &newState)				{ currentState = &newState; }
private:
	NPCState* currentState;
	float speed = 9.0f;
	std::vector<Vector3> path;
	Pathfinding* pathing;
	std::shared_ptr<Player> player;                                                                                                                                                                                                                                                                
};