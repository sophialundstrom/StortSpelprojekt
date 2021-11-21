#pragma once
#include "NPCHostile.h"

class BarbarianCamp
{
public:
	enum class Location { South, East, North, West, Village };
	static const std::string Locations[];
private:
	float radius;
	Location location;
	UINT numBarbarians;
	std::vector<std::shared_ptr<HostileNPC>> barbarians;
public:
	BarbarianCamp(Location location, float radius);
	~BarbarianCamp();

	void AddBarbarian(const std::string& file, const Vector3& position, bool dynamic = true);

	Location GetLocation();
	UINT NumDead();
	UINT NumBarbarians();

	void Reset();
};

inline const std::string BarbarianCamp::Locations[] = { "Southern", "Eastern", "Northern", "Western", "Village" };