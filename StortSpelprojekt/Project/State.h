#pragma once
#include "NPCHostile.h"
#include "DirectXHelp.h"
#include "Time.h"
#include "Math.h"

enum State
{
	IDLING,
	MOVING,
	SHOOTING,
	SEARCHING//MAYBE
};

class HostileNPC;

class NPCState
{
public:
	//NPCState();
	virtual ~NPCState();
	//virtual void handleInput(HostileNPC& heroine, Input input) {}
	virtual void update(HostileNPC& heroine);

	static MovingState moving;
	static IdlingState idling;
	static ShootingState shooting;
private:

};

class MovingState : NPCState
{
public:
	MovingState();
	virtual void update(HostileNPC& hostile);
};

class ShootingState : NPCState
{
private:
	CombatStyle combatStyle;
    float shootDeelay = 0.2f;
    float lastShot = 0.f;


    float shootDeelayPattern[3];
    int shootPatternIndex = 0;

    int enemyShootDetectionRadius = 150;
    float movementXRadiant;
    float movementYRadiant;
public:
	ShootingState() {}
	void SwapCombatStyle(CombatStyle newCombatStyle);
	void update(HostileNPC& hostile);
};

class IdlingState : public NPCState
{
	IdlingState();
	virtual void update(HostileNPC& hostile);
};