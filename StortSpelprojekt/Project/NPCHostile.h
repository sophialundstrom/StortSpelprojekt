#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;

struct CampData
{
	enum class Location { South, East, North, West, Village };
	std::map<Location, int> locationCutoff;
	CampData()
	{
		locationPosition.emplace(Location::South, Vector3(-617.626160f, 70.000000f, -252.823212f));
		locationPosition.emplace(Location::East, Vector3(0, 0, 0));
		locationPosition.emplace(Location::North, Vector3(0, 0, 0));
		locationPosition.emplace(Location::West, Vector3(0, 0, 0));
		locationPosition.emplace(Location::Village, Vector3(0, 0, 0));

		locationCutoff.emplace(Location::South, 160);
		locationCutoff.emplace(Location::East, 160);
		locationCutoff.emplace(Location::North, 160);
		locationCutoff.emplace(Location::West, 160);
		locationCutoff.emplace(Location::Village, 160);

	}
	std::map<Location, Vector3> locationPosition;

	static const std::string Locations[];
};
inline const std::string CampData::Locations[] = { "Southern", "Eastern", "Northern", "Western", "Village" };

class HostileNPC : public NPC
{
private:
	
	//std::vector<Vector3> path;
	std::shared_ptr<Pathfinding> pathing;
	std::shared_ptr<Player> player;
	ArrowHandler arrowHandler;
	int damage;
	void CalcHeight(HeightMap* heightMap);
	CampData campData;
public:
	bool isMoving;
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, const Vector3& targetPosition, std::shared_ptr<Pathfinding> pathing, int health, bool moving);
;
	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player, HeightMap* heightMap);

	ArrowHandler& GetArrowHandler() { return this->arrowHandler; }

	void Reset();
	void SetSpawnPosition(const Vector3& position);

	void CheckPlayerCollision(std::shared_ptr<Player> player);

	std::shared_ptr<Player> GetPlayer() { return this->player; }

	std::shared_ptr<Pathfinding> GetPath()	{ return this->pathing; }
	CampData GetCampData()					{ return this->campData;}

	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetState(NPCState& newState);

	void SetDamage(int x)							{ damage = x; }

	float lastShot = 0.f;
	int pathIndex = -1;
	Quaternion originalRotation;
	std::vector<Vector3> path;
	Vector3 targetPosition;
	float speed = 9.0f;
	float heightMapGroundLevel = 0.f;
	NPCState* currentState;
	float viewDistance;
	float distanceToPlayer;
	Vector3 startPos;

                                                                                                                                                                                                                                                             
};

