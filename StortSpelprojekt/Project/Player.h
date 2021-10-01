#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"
#include "Terrain.h"

typedef enum RESOURCES
{
	WOOD,
	STONE,
	FOOD,
	NONE
};

struct Inventory 
{
	std::map<RESOURCES, UINT> items; //ID , NUM OF ITEM
	std::map<RESOURCES, std::string> names;

	Inventory()
	{
		names[RESOURCES::WOOD] = "Wood";
		names[RESOURCES::STONE] = "Stone";
		names[RESOURCES::FOOD] = "Food";
		names[RESOURCES::NONE] = "NONE";
	}
	
	void AddItem(enum RESOURCES ID)
	{
		items[ID]++;
	}

	void RemoveItem(enum RESOURCES ID)
	{	
		if (items[ID] == 1)
		{
			items.erase(ID);
			return;
		}

		items[ID]--;
	}

	UINT NumOf(enum RESOURCES ID)
	{
		return items[ID];
	}

	void GetResources(enum RESOURCES ID)
	{
		std::cout << names[ID] << " " << items[ID] << std::endl;
	}
};

class Player : public Model
{
public:
	
	struct GameStats
	{
		//MOSTLY FOR QUEST PROGRESS BUT MAYBE FUN TO KNOW WHEN GAME IS OVER?
		UINT barbariansKilled = 0;
	}gameStats;

	struct Stats
	{
		UINT barbariansKilled = 0;
		float movementSpeed = 5.0f;
		float sprintSpeed = 10.0f;
		UINT maxHealthPoints = 10.0f;
		UINT healthPoints = 0.0f;
		UINT level = 1;
		float currentSpeed = movementSpeed;
		void SetMaxHealthPoints(UINT newMaxHealthPoints) { this->maxHealthPoints = newMaxHealthPoints; }
		void SetHealthPoints(UINT newHealthPoints) { this->healthPoints = newHealthPoints; }
		void SetMovementSpeed(float newMovementSpeed) { this->movementSpeed = newMovementSpeed; }
		void SetSprintSpeed(float newSprintSpeed) { this->sprintSpeed = newSprintSpeed; }
		void IncreaseHealthPoints() { this->healthPoints++; };
		void DecreaseHealthPoint() { this->healthPoints--; };
		
	} stats;

	// TEMP STATS PRINT
	void GetStats()
	{
		std::cout << "---------------------PLAYER STATS--------------------- " << std::endl;
		std::cout << "LEVEL " << stats.level << std::endl;
		std::cout << "MAXHEALTH " << stats.maxHealthPoints << std::endl;
		std::cout << "CURRENT HEALTH " << stats.healthPoints << std::endl;
		std::cout << "CURRENT MOVEMENTSPEED " << stats.currentSpeed << std::endl;
		std::cout << "BARBARIANS KILLED " << gameStats.barbariansKilled << std::endl;
	}

private:
	Camera* sceneCamera;

	float movementOfsetRadiant = 0;

	float preJumpGroundLevel = 0;
	float heightMapGroundLevel = 20.0f;
	float mouseSensitivity = 10.f;
	//float camDistance = 10;//How far the camera is from the playerboject

	float gravity = 9.82f;
	float timePassed = 0;

	bool jumping = false;
	bool maxJumpHeight = false;
	bool pressed = false;
	bool sprint= false;

	float airTime = 0;
	float jumpHeight = 5.0f;

	float defaultCameraDistance = 13.0f;
	float currentCameraDistance = defaultCameraDistance;
	float maxCameraDistance = defaultCameraDistance + 5.0f;
	
	float get2dAngle(Vector2 a, Vector2 b)
	{
		//MathExplanation
		//https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors

		a.Normalize();
		b.Normalize();

		return acos(a.x * b.x + a.y * b.y);
	};

	void CalcHeight(HeightMap* heightMap)
	{
		const int lowX = (int)std::floor(position.x);
		const int highX = (int)std::ceil(position.x);
		const float Xdecimal = position.x - lowX;

		const int lowZ = (int)std::floor(position.z);
		const int highZ = (int)std::ceil(position.z);
		const float Zdecimal = position.z - lowZ;

		const float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
		const float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
		const float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
		const float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

		heightMapGroundLevel = position.y = H1 + H2 + H3 + H4;
	}

	Inventory inventory;
public:
	void Update(HeightMap* heightMap)
	{
		CalcHeight(heightMap);

		//Rotate camera by cursor movement 
		sceneCamera->Rotate(Event::ReadRawDelta().x * mouseSensitivity, Event::ReadRawDelta().y * mouseSensitivity);

		//Get players position last frame and cameras current look-direction
		Vector3 lookDirection = sceneCamera->GetDirection();

		//MOVEMENT DIRECTION
		Vector3 moveDirection;

		if (Event::KeyIsPressed('W'))
			moveDirection += Vector3(0, 0, 1);
		if (Event::KeyIsPressed('S'))
			moveDirection += Vector3(0, 0, -1);
		if (Event::KeyIsPressed('A'))
			moveDirection += Vector3(-1, 0, 0);
		if (Event::KeyIsPressed('D'))
			moveDirection += Vector3(1, 0, 0);

		moveDirection.Normalize();

		//SPRINTING
		if (Event::KeyIsPressed(VK_SHIFT))
		{
			stats.currentSpeed += 5.0f * Time::GetDelta();
			if (stats.currentSpeed > stats.sprintSpeed)
				stats.currentSpeed = stats.sprintSpeed;

			currentCameraDistance += Time::GetDelta() * 10.0f;
			if (currentCameraDistance > maxCameraDistance)
				currentCameraDistance = maxCameraDistance;
		}
			
		else
		{
			stats.currentSpeed -= 12.0f * Time::GetDelta();
			if (stats.currentSpeed < stats.movementSpeed)
				stats.currentSpeed = stats.movementSpeed;

			currentCameraDistance -= Time::GetDelta() * 7.0f;
			if (currentCameraDistance < defaultCameraDistance)
				currentCameraDistance = defaultCameraDistance;
		}
			
	
		//Calculate playerJumpVelocity
		//jumpVelocity = sqrtf(2 * gravity * 1);
		
		//Calculate the radians between the cameras yAxis direction and {0, 0, 1}-Vector.
		//Aligns the keyboardinputs by the camera direction afterwards via the radian.

		if (!Event::RightIsClicked())
		{
			movementOfsetRadiant = get2dAngle({ lookDirection.x, lookDirection.z }, { 0, 1 });
			if (lookDirection.x < 0)
				movementOfsetRadiant *= -1;
		}

		Matrix movementOfsetMatrix = Matrix::CreateRotationY(movementOfsetRadiant);
		moveDirection = Vector3::Transform(moveDirection, movementOfsetMatrix);

		//Only update what direction the player is facing when keyboardinput is detected by the moveDirection vector
		if (moveDirection.Length() > 0 || moveDirection.Length() < 0)
			rotation = { 0, movementOfsetRadiant + PI, 0 };

		//Updates the player and cameras positions
		moveDirection = moveDirection * stats.currentSpeed * Time::GetDelta();
		Vector3 newPlayerPos = position + moveDirection;

		// JUMPING 
		if (!jumping)
		{
			if (Event::KeyIsPressed(VK_SPACE))
			{
				jumping = true;
				preJumpGroundLevel = heightMapGroundLevel;
			}
		}
		
		if (jumping)
		{
			airTime += Time::GetDelta() * 5.0f;
			newPlayerPos.y = -powf(airTime, 2) + jumpHeight * airTime + preJumpGroundLevel;
		}

		// RESET TO THE "GROUND"
		if (newPlayerPos.y < heightMapGroundLevel)
		{
			airTime = 0;
			pressed = false;
			jumping = false;
			newPlayerPos = Vector3(newPlayerPos.x, heightMapGroundLevel, newPlayerPos.z);
		}

		position = newPlayerPos + Vector3(0, 1, 0);

		Vector3 newCameraPos = position + (lookDirection * -currentCameraDistance);

		sceneCamera->MoveTowards(newCameraPos);

		Model::Update();
	};


	Player(Camera* camera)
		:Model("PlayerArrow", "PlayerArrow"), sceneCamera(camera)
	{
		rotation = { 0, PI, 0 };
	}

	Inventory& Inventory() { return inventory; }
	GameStats& GameStats() { return gameStats; }
};