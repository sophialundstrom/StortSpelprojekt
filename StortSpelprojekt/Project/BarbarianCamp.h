#pragma once
#include "NPCHostile.h"
#include "Scene.h"
#include "Pathfinding.h"

class BarbarianCamp
{
public:

private:
	bool active;
	float radius;
	Location location;
	UINT numBarbarians;
	Vector3 target;
	Vector3 position;
public:
	std::vector<std::shared_ptr<HostileNPC>> barbarians;
	BarbarianCamp() = default;
	BarbarianCamp(const Vector3& position, Location location, float radius, bool active = true);

	void AddBarbarian(const std::string& file, const Vector3& position, std::vector<std::shared_ptr<HostileNPC>>& hostiles, std::shared_ptr<Player> player,	CombatStyle combatStyle, const Vector3& targetPosition, std::shared_ptr<Pathfinding> pathing, bool dynamic = true, int damage = 1, int health = 3, bool moving = true);

	Location GetLocation();
	UINT NumDead();
	UINT NumBarbarians();
	void SetTarget(const Vector3& target);

	void Update(std::shared_ptr<Player> player, HeightMap* heightMap);

	void Reset();
};

