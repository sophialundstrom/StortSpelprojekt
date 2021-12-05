#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;

class HostileNPC : public NPC
{
private:
	
	//std::vector<Vector3> path;
	std::shared_ptr<Pathfinding> pathing;
	std::shared_ptr<Player> player;
	ArrowHandler arrowHandler;
	int damage;
	void CalcHeight(HeightMap* heightMap);
public:
	bool isMoving;
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, const Vector3& targetPosition, std::shared_ptr<Pathfinding> pathing, int health, bool moving);
;
	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player, HeightMap* heightMap);

	ArrowHandler& GetArrowHandler() { return this->arrowHandler; }

	void Reset();
	void SetSpawnPosition(const Vector3& position);

	void CheckPlayerCollision(std::shared_ptr<Player> player);

	std::shared_ptr<Player> GetPlayer() { return this->player; }

	std::shared_ptr<Pathfinding> GetPath()			{ return this->pathing; }

	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetState(NPCState& newState);// { currentState = &newState; }

	void SetDamage(int x)							{ damage = x; }

	float lastShot = 0.f;
	int pathIndex = -1;
	Quaternion originalRotation;
	std::vector<Vector3> path;
	Vector3 targetPosition;
	float speed = 9.0f;
	float heightMapGroundLevel = 0.f;
	NPCState* currentState;
	float viewDistance;
	float distanceToPlayer;

                                                                                                                                                                                                                                                             
};