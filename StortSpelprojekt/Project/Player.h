#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"
#include "Terrain.h"
#include "Item.h"

struct Inventory 
{
	//std::unordered_map here?
	std::map<RESOURCE, UINT> items; //ID , NUM OF ITEM
	std::map<RESOURCE, std::string> names;

	Inventory()
	{
		names[RESOURCE::WOOD] = "Wood";
		names[RESOURCE::STONE] = "Stone";
		names[RESOURCE::FOOD] = "Food";
		names[RESOURCE::NONE] = "NONE";
	}
	
	void AddItem(enum RESOURCE ID)
	{
		items[ID]++;
	}

	void RemoveItem(enum RESOURCE ID)
	{	
		if (items[ID] == 1)
		{
			items.erase(ID);
			return;
		}

		items[ID]--;
	}

	UINT NumOf(enum RESOURCE ID)

	{
		return items[ID];
	}

	void GetResources(enum RESOURCE ID)
	{
		std::cout << names[ID] << " " << items[ID] << std::endl;
	}
};

struct Stats
{
	UINT barbariansKilled = 0;
	float movementSpeed = 5.0f;
	float sprintSpeed = 10.0f;
	UINT maxHealthPoints = 10;
	UINT healthPoints = 0;
	UINT level = 1;
	float currentSpeed = movementSpeed;

	void SetMaxHealthPoints(UINT newMaxHealthPoints) { this->maxHealthPoints = newMaxHealthPoints; }
	void SetHealthPoints(UINT newHealthPoints) { this->healthPoints = newHealthPoints; }
	void SetMovementSpeed(float newMovementSpeed) { this->movementSpeed = newMovementSpeed; }
	void SetSprintSpeed(float newSprintSpeed) { this->sprintSpeed = newSprintSpeed; }
	void IncreaseHealthPoints() { this->healthPoints++; };
	void DecreaseHealthPoint() { this->healthPoints--; };
};

class Player : public Model
{
private:
	Stats stats;

	Camera* sceneCamera;

	float movementOfsetRadiant = 0;

	float preJumpGroundLevel = 0;
	float heightMapGroundLevel = 20.0f;
	float mouseSensitivity = 10.f;

	float gravity = 9.82f;
	float timePassed = 0;

	bool jumping = false;
	bool maxJumpHeight = false;
	bool pressed = false;
	bool sprint = false;

	float airTime = 0;
	float jumpHeight = 5.0f;

	float defaultCameraDistance = 13.0f;
	float currentCameraDistance = defaultCameraDistance;
	float maxCameraDistance = defaultCameraDistance + 5.0f;
	
	Vector3 cameraLocationSocket = { 1, 4, 0 };

	void CalcHeight(HeightMap* heightMap);
	void Load(std::string file);
	std::shared_ptr<BoundingSphere> bounds;
	Inventory inventory;
public:
	void Update(HeightMap* heightMap);
	
	Player(const std::string file, Camera* camera)
		:Model("Character", "Character"), sceneCamera(camera)
	{
		bounds = std::make_shared<BoundingSphere>();

		SetScale(0.02f);
		bounds->SetScale(200);

		Load(file);
	}
public:
	// TEMP STATS PRINT
	void GetStats()
	{
		std::cout << "---------------------PLAYER STATS--------------------- " << std::endl;
		std::cout << "LEVEL " << stats.level << std::endl;
		std::cout << "MAXHEALTH " << stats.maxHealthPoints << std::endl;
		std::cout << "CURRENT HEALTH " << stats.healthPoints << std::endl;
		std::cout << "CURRENT MOVEMENTSPEED " << stats.currentSpeed << std::endl;
		std::cout << "BARBARIANS KILLED " << stats.barbariansKilled << std::endl;
	}
	std::shared_ptr<BoundingSphere> GetBounds(){ return bounds; }

	Inventory& Inventory() { return inventory; }
	Stats& Stats() { return stats; }

	void Save(const std::string file);
};