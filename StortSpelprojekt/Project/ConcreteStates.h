#pragma once
#include "State.h"
#include "Pathfinding.h"

class MovingState : public NPCState
{
public:
	
	void Enter(HostileNPC& hostile);
	void Update(HostileNPC& hostile);
	void Exit(HostileNPC& hostile);
	static NPCState& GetInstance();
private:
	MovingState() {}
	MovingState(const MovingState& other);
	MovingState& operator=(const MovingState& other);


	float speed = 16.0f;
};

class ShootingState : public NPCState
{
public:
	ShootingState() {}
	void SwapCombatStyle(CombatStyle newCombatStyle);
	void Enter(HostileNPC& hostile);
	void Update(HostileNPC& hostile);
	void Exit(HostileNPC& hostile);
	static NPCState& GetInstance();
private:
	CombatStyle combatStyle;
	float shootDeelay = 0.2f;
//	float lastShot = 0.f;


	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

	int enemyShootDetectionRadius = 100;
	float movementXRadiant;
	float movementYRadiant;
};

class IdlingState : public NPCState
{
public:
	void Enter(HostileNPC& hostile);
	void Update(HostileNPC& hostile);
	void Exit(HostileNPC& hostile);
	static NPCState& GetInstance();
private:
	IdlingState() {}
	IdlingState(const IdlingState& other);
	IdlingState& operator=(const IdlingState& other);
};