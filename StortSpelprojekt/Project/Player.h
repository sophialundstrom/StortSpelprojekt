#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"
#include "Terrain.h"
#include "Item.h"
#include "Canvas.h"
#include "ArrowHandler.h"
#include "AnimatedModel.h"
#include "Biome.h"
#include "Inventory.h"

class Building;

struct Stats
{
	UINT barbariansKilled = 0;
	float movementSpeed = 20.0f;
	float sprintSpeed = 60.0f;
	UINT maxHealthPoints = 10;
	UINT healthPoints = 10;
	UINT level = 1;
	float currentSpeed = movementSpeed;
	int resist = 0;
	int damage = 1;
	int HPGain = 1;

	void SetMaxHealthPoints(UINT newMaxHealthPoints)	{ this->maxHealthPoints = newMaxHealthPoints; }
	void SetHealthPoints(UINT newHealthPoints)			{ this->healthPoints = newHealthPoints; }
	void SetMovementSpeed(float newMovementSpeed)		{ this->movementSpeed = newMovementSpeed; }
	void SetSprintSpeed(float newSprintSpeed)			{ this->sprintSpeed = newSprintSpeed; }
	void IncreaseHealthPoints()
	{ 
		if (healthPoints < maxHealthPoints)
		{
			this->healthPoints += HPGain; 
			if (this->healthPoints > 10)
				this->healthPoints = 10;
		}
	}

	void DecreaseHealthPoint() { if (healthPoints != 0) this->healthPoints--; };
};

class Player : public AnimatedModel
{
private:
	Stats stats;

	std::shared_ptr<AnimatedModel> bow;

	std::shared_ptr<Canvas> ingameCanvas;

	ArrowHandler arrowHandler;

	bool hasCollided;

	float preJumpGroundLevel = 0.0f;
	float currentGroundLevel = 0.0f;
	float heightMapGroundLevel = 0.0f;

	float movementYRadiant = 0.0f;
	float movementXRadiant = 0.0f;

	const float mouseDefaultSensitivity = 10.f;
	const float mouseAimSensitivity = 4.f;
	float mouseCurrentSensitivity = mouseDefaultSensitivity;

	float gravity = 9.82f;
	float timePassed = 0;

	bool jumping = false;
	bool maxJumpHeight = false;
	bool pressed = false;
	bool isSprinting = false;

	float airTime = 0;
	float jumpHeight = 5.0f;

	float defaultCameraDistance = 17.0f;
	float currentCameraDistance = defaultCameraDistance;
	float maxCameraDistance = defaultCameraDistance + 7.0f;
	float minCameraDistance = 0.5f;

	float closestColliderToCam = 9999;
	Vector3 cameraLocationSocket = { -1.3f, 8.0, -4.f };

	void CalcHeight(HeightMap* heightMap);
	float CalcHeightForCamera(HeightMap* heightMap);
	void Load(std::string file);

	std::shared_ptr<BoundingBox> bounds;
	std::shared_ptr<FrustumCollider> frustum;

	bool gameOver = false;

	Inventory inventory;

	Vector3 lastPosition = Vector3(0, 0, 0);

	bool isAiming = false;
public:
	Camera* sceneCamera;
	BIOME currentBiome;
	BIOME previousBiome;
	
	UINT maxArrows = 10;
	UINT numArrows = 5;

	void Update(HeightMap* heightMap);
	ArrowHandler GetArrowHandler() { return this->arrowHandler; }
	void TakeDamage(int x);
	bool inCombat = false;
	void SwitchBiomeMusic();
	void SetDamage(int x) { stats.damage = x; }

	Player(const std::string file, Camera* camera, const UINT& maxArrows);

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

	bool GetGameOver() { return this->gameOver; }
	std::shared_ptr<BoundingBox> GetBounds() { return bounds; }
	std::shared_ptr<FrustumCollider> GetFrustum() { return frustum; }

	Inventory& Inventory() { return inventory; }
	Stats& Stats() { return stats; }

	void Save(const std::string file);

	void HandleCollidedObjects(const std::vector<std::shared_ptr<Collider>> colliders);
	void ResetToLastPosition() { position = lastPosition; }
	void AddHealthPoint() { stats.IncreaseHealthPoints(); }
	void SetClosestColliderToCam(float range)
	{
		closestColliderToCam = range;
	}
	void HandleUpgrades(std::shared_ptr<Building> building);
};