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

#undef Ray

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

	void RemoveItem(enum RESOURCE ID, UINT amount = 1)
	{
		if (items[ID] <= amount)
		{
			items[ID] = 0;
			return;
		}

		items[ID] -= amount;
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
	float movementSpeed = 20.0f;
	float sprintSpeed = 70.0f;
	UINT maxHealthPoints = 1;
	UINT healthPoints = 1;
	UINT level = 1;
	float currentSpeed = movementSpeed;

	void SetMaxHealthPoints(UINT newMaxHealthPoints) { this->maxHealthPoints = newMaxHealthPoints; }
	void SetHealthPoints(UINT newHealthPoints) { this->healthPoints = newHealthPoints; }
	void SetMovementSpeed(float newMovementSpeed) { this->movementSpeed = newMovementSpeed; }
	void SetSprintSpeed(float newSprintSpeed) { this->sprintSpeed = newSprintSpeed; }
	void IncreaseHealthPoints() { if (healthPoints < maxHealthPoints) this->healthPoints++; };
	void DecreaseHealthPoint() { if (healthPoints != 0) this->healthPoints--; };
};

class Player : public AnimatedModel
{
private:
	Stats stats;

	Camera* sceneCamera;

	std::shared_ptr<Canvas> ingameCanvas;

	//std::vector<std::shared_ptr<Arrow>>arrows;
	ArrowHandler arrowHandler;

	bool hasCollided;

	float movementYRadiant = 0;
	float movementXRadiant = 0;

	float preJumpGroundLevel = 0.0f;
	float currentGroundLevel = 0.0f;
	float heightMapGroundLevel = 0.0f;

	const float mouseDefaultSensitivity = 3.f;
	const float mouseAimSensitivity = 2.f;
	float mouseCurrentSensitivity = mouseDefaultSensitivity;
	float mouseSensitivity = 5.f;

	float gravity = 9.82f;
	float timePassed = 0;

	bool jumping = false;
	bool maxJumpHeight = false;
	bool pressed = false;
	bool sprint = false;

	float airTime = 0;
	float jumpHeight = 5.0f;

	float defaultCameraDistance = 17.0f;
	float currentCameraDistance = defaultCameraDistance;
	float maxCameraDistance = defaultCameraDistance + 7.0f;
	float closestColliderToCam = 9999;
	Vector3 cameraLocationSocket = { 1.3f, 5.0, -2.f };

	void CalcHeight(HeightMap* heightMap);
	float CalcHeightForCamera(HeightMap* heightMap);
	void Load(std::string file);

	std::shared_ptr<BoundingBox> bounds;
	std::shared_ptr<FrustumCollider> frustum;

	bool isRightPressed;
	bool isLeftPressed;
	bool gameOver = false;

	Inventory inventory;

	Vector3 lastPosition = Vector3(0, 0, 0); // 
	float sinceLastShot = 0;
	float shootingAnimationLenght = 1.f;
	float currentLerp = 0.f;
	float duration = 1.f;
	bool inAir = false;

	void UpdateHealthUI()
	{
		auto image = ingameCanvas->GetImage("hp");
		if (image->FileName() == "HP" + std::to_string(stats.healthPoints) + ".png")
			return;

		auto position = image->GetPosition();
		ingameCanvas->RemoveImage("hp");
		ingameCanvas->AddImage(position, "hp", "HP" + std::to_string(stats.healthPoints) + ".png");
	}

public:
	UINT maxArrows = 10;
	UINT numArrows = 5;
	void Update(HeightMap* heightMap, ModelRenderer& mRenderer, ColliderRenderer& cRenderer);
	ArrowHandler GetArrowHandler() { return this->arrowHandler; }
	void TakeDamage();

	Player(const std::string file, Camera* camera, std::shared_ptr<Canvas> ingameCanvas/*, std::vector<std::shared_ptr<Arrow>> arrows*/, const UINT& maxArrows);

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
	void AddHealthPoint() { stats.IncreaseHealthPoints(); UpdateHealthUI(); }
	void SetClosestColliderToCam(float range)
	{
		closestColliderToCam = range;
	}
};