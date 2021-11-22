#pragma once
#include "NPCHostile.h"

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
	virtual void update(HostileNPC& heroine) {}
private:

};
