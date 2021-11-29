#pragma once
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

enum CombatStyle
{
	consistantDelay,
	Burst,
	wideArrow,
	Defenseless
};

class HostileNPC;

class NPCState
{
public:
	//NPCState();
	virtual ~NPCState() {};
	virtual void Enter(HostileNPC& hostile) = 0;
	virtual void Update(HostileNPC& hostile) = 0;
	virtual void Exit(HostileNPC& hostile) = 0;
};
