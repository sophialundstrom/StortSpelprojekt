#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"
#include "Terrain.h"
#include "Item.h"
#include "Canvas.h"
#include "Arrow.h"

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
	float movementSpeed = 5.0f;
	float sprintSpeed = 10.0f;
	UINT maxHealthPoints = 10;
	UINT healthPoints = 10;
	UINT level = 1;
	float currentSpeed = movementSpeed;

	void SetMaxHealthPoints(UINT newMaxHealthPoints) { this->maxHealthPoints = newMaxHealthPoints; }
	void SetHealthPoints(UINT newHealthPoints) { this->healthPoints = newHealthPoints; }
	void SetMovementSpeed(float newMovementSpeed) { this->movementSpeed = newMovementSpeed; }
	void SetSprintSpeed(float newSprintSpeed) { this->sprintSpeed = newSprintSpeed; }
	void IncreaseHealthPoints() { if (healthPoints < maxHealthPoints) this->healthPoints++; };
	void DecreaseHealthPoint() { if (healthPoints != 0) this->healthPoints--; };
};

class Player : public Model
{
private:
	Stats stats;

	Camera* sceneCamera;
	
	Canvas* ingameCanvas;
	//ARROW STUFF
	std::vector<std::shared_ptr<Arrow>> arrows;

	float movementYRadiant = 0;
	float movementXRadiant = 0;

	float preJumpGroundLevel = 0;
	float heightMapGroundLevel = 20.0f;
	const float mouseDefaultSensitivity = 2.f;
	const float mouseAimSensitivity = 5.f;
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
	
	Vector3 cameraLocationSocket = { 1.3, 2.7f, -2.f };

	void CalcHeight(HeightMap* heightMap);
	void Load(std::string file);

	std::shared_ptr<RayCollider> ray;
	std::shared_ptr<BoundingSphere> bounds;
	std::shared_ptr<FrustumCollider> frustum;

	bool isRightPressed;
	bool isLeftPressed;

	Inventory inventory;

	void UpdateHealthUI()
	{
		for (UINT i = 0; i < stats.maxHealthPoints; ++i)
		{	
			const std::string name = "hp" + std::to_string(i);
			auto image = ingameCanvas->GetImage(name);

			if (image->FileName() != "Heart.png" && stats.healthPoints > 0)
			{
				auto position = image->GetPosition();
				ingameCanvas->RemoveImage(name);
				ingameCanvas->AddImage(position, name, "Heart.png");
			}

			if (i == 0 && stats.healthPoints == 0)
			{
				auto position = image->GetPosition();
				ingameCanvas->RemoveImage(name);
				ingameCanvas->AddImage(position, name, "RedHeart.png");
			}

			if (i <= stats.healthPoints)
				image->Show();
			else
				image->Hide();
		}
	}
public:
	void Update(HeightMap* heightMap);
	
	Player(const std::string file, Camera* camera, Canvas* ingameCanvas, std::vector<std::shared_ptr<Arrow>> arrows)
		:Model("LowPolyCharacter", "Player"), sceneCamera(camera), ingameCanvas(ingameCanvas)
	{
		isRightPressed = false;
		isLeftPressed = false;

		this->arrows = arrows;
		bounds = std::make_shared<BoundingSphere>();
		ray = std::make_shared<RayCollider>();
		ray->length = 40;

		bounds->SetScale(3);

		frustum = std::make_shared<FrustumCollider>(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);

		Load(file);
		UpdateHealthUI();
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
	std::shared_ptr<RayCollider> GetRay() { return ray; }
	std::shared_ptr<FrustumCollider> GetFrustum() { return frustum; }

	Inventory& Inventory() { return inventory; }
	Stats& Stats() { return stats; }




	void Save(const std::string file);

	void TakeDamage() { stats.DecreaseHealthPoint(); UpdateHealthUI(); }
	void AddHealthPoint() { stats.IncreaseHealthPoints(); UpdateHealthUI(); }
};