#pragma once
#include "NPCHostile.h"
#include "Scene.h"

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
	BarbarianCamp() = default;
	BarbarianCamp(Location location, float radius);

	void AddBarbarian(const std::string& file, const Vector3& position, std::vector<std::shared_ptr<HostileNPC>>& hostiles, std::shared_ptr<Player> player, CombatStyle combatStyle, bool dynamic = true);

	Location GetLocation();
	UINT NumDead();
	UINT NumBarbarians();

	void Reset();
};

inline const std::string BarbarianCamp::Locations[] = { "Southern", "Eastern", "Northern", "Western", "Village" };