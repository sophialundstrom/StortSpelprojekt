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

class HuntingState : public NPCState
{
public:

	void Enter(HostileNPC& hostile);
	void Update(HostileNPC& hostile);
	void Exit(HostileNPC& hostile);
	static NPCState& GetInstance();
private:
	HuntingState() {}
	HuntingState(const HuntingState& other);
	HuntingState& operator=(const HuntingState& other);

	float speed = 16.0f;
};

class ShootingState : public NPCState
{
public:
	void SwapCombatStyle(CombatStyle newCombatStyle);
	ShootingState() {}
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

	int enemyShootDetectionRadius = 200;
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

// for barb guards in towers
class GuardState : public NPCState
{
public:
	void Enter(HostileNPC& hostile);
	void Update(HostileNPC& hostile);
	void Exit(HostileNPC& hostile);
	static NPCState& GetInstance();
private:
	GuardState() {}
	GuardState(const GuardState& other);
	GuardState& operator=(const GuardState& other);

	CombatStyle combatStyle;
	float shootDeelay = 0.2f;
	//	float lastShot = 0.f;


	float shootDeelayPattern[3];
	int shootPatternIndex = 0;

	int enemyShootDetectionRadius = 200;
	float movementXRadiant;
	float movementYRadiant;

	void SwapCombatStyle(CombatStyle newCombatStyle)
	{
		combatStyle = newCombatStyle;
		shootPatternIndex = 0;

		float normalDelay = 2.f;
		float quickDelay = 0.2f;
		float breakTime = 2.4f;
		float superSlow = 864000;

		switch (combatStyle)
		{
		case CombatStyle::consistantDelay:
			shootDeelayPattern[0] = normalDelay;
			shootDeelayPattern[1] = normalDelay;
			shootDeelayPattern[2] = normalDelay;
			break;

		case CombatStyle::Burst:
			shootDeelayPattern[0] = quickDelay;
			shootDeelayPattern[1] = quickDelay;
			shootDeelayPattern[2] = quickDelay + breakTime;
			break;

		case CombatStyle::Defenseless:
			shootDeelayPattern[0] = superSlow;
			shootDeelayPattern[1] = superSlow;
			shootDeelayPattern[2] = superSlow;
			break;

		default:
			break;
		}
	}
};