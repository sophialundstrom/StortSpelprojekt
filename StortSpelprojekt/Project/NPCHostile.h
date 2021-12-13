#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;


struct CampData
{
	enum class Location { South, East, North, West, Village };
	CampData();

	static std::map<Location, Vector3> locationPosition;
	static std::map<Location, int> locationCutoff;

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
	bool haveCheckedHeight = false;

	void CalcHeight(HeightMap* heightMap);
public:
	bool isMoving;
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, const Vector3& targetPosition, std::shared_ptr<Pathfinding> pathing, int health, bool moving);
;
	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player, HeightMap* heightMap);


	void Reset();
	void SetSpawnPosition(const Vector3& position);

	void CheckPlayerCollision(std::shared_ptr<Player> player);

	ArrowHandler& GetArrowHandler()			{ return this->arrowHandler; }
	std::shared_ptr<Player> GetPlayer()		{ return this->player; }
	std::shared_ptr<Pathfinding> GetPath()	{ return this->pathing; }
	CampData::Location GetCampData()		{ return this->assignedCampLocation;}

	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetDamage(int x)							{ this->damage = x; }
	void SetState(NPCState& newState);
	// set position and various campdata
	void SetPosition(Vector3 pos, Vector3 campPos, int cutoff, CampData::Location);


	int		pathIndex = -1;
	int		campCutOff;
	float	lastShot = 0.f;
	float	speed = 9.0f;
	float	heightMapGroundLevel = 0.f;
	float	viewDistance;
	float	distanceToPlayer;
	Vector3 targetPosition;
	Vector3 startPos;
	Vector3 assignedCampPos;
	NPCState* currentState;
	Quaternion originalRotation;
	CampData::Location assignedCampLocation;
	std::vector<Vector3> path;

                                                                                                                                                                                                                                                             
};

