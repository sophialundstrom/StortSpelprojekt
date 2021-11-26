#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;

class HostileNPC : public NPC
{
private:
	NPCState* currentState;
	float speed = 9.0f;
	std::vector<Vector3> path;
	Pathfinding* pathing;
	std::shared_ptr<Player> player;
	ArrowHandler arrowHandler;
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle);
;
	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player);

	ArrowHandler GetArrowHandler() { return this->arrowHandler; }

	void Reset();
	void SetSpawnPosition(const Vector3& position);

	void CheckPlayerCollision(std::shared_ptr<Player> player);

	std::shared_ptr<Player> GetPlayer() { return this->player; }

	void SetPathVar(Pathfinding* path)				{ this->pathing = path; }
	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetState(NPCState &newState)				{ currentState = &newState; }
                                                                                                                                                                                                                                                             
};