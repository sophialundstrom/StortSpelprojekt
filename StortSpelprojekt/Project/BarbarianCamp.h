#pragma once
#include "NPCHostile.h"

class BarbarianCamp
{
public:
	enum class Location { South, East, North, West, Village };
	static const std::string Locations[];
private:
	Location location;
	UINT numBarbarians;
	std::shared_ptr<HostileNPC>* barbarians;
public:
	BarbarianCamp(Location location, UINT numBarbarians, float radius);
	~BarbarianCamp();

	Location GetLocation();
	UINT NumDead();
	UINT NumBarbarians();
};

inline const std::string BarbarianCamp::Locations[] = { "Southern", "Eastern", "Northern", "Western", "Village" };