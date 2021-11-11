#pragma once
#include "NPCHostile.h"

class BarbarianCamp
{
public:
	static const UINT NUM_BARBARIANS = 3;
private:
	UINT ID;
	std::shared_ptr<HostileNPC> barbarians[NUM_BARBARIANS];
public:
	UINT GetID();
	UINT NumDead();
};