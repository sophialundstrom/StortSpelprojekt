#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"
#include "Terrain.h"

struct Inventory 
{ 
	std::map<UINT, UINT> items; //ID , NUM OF ITEM

	Inventory() = default;

	void AddItem(UINT ID)
	{
		items[ID]++;
	}

	void RemoveItem(UINT ID)
	{	
		if (items[ID] == 1)
		{
			items.erase(ID);
			return;
		}

		items[ID]--;
	}

	UINT NumOf(UINT ID)
	{
		return items[ID];
	}
};

struct GameStats
{
	//MOSTLY FOR QUEST PROGRESS BUT MAYBE FUN TO KNOW WHEN GAME IS OVER?
	UINT barbariansKilled = 0;
};

class Player : public Model
{
private:
	Camera* sceneCamera;

	struct Stats
	{
		float movementSpeed;
		UINT health;
		//OSV
	} stats;

	float movementOfsetRadiant = 0;

	float walkSpeed = 5.0f;
	float sprintSpeed = 10.0f;
	float currentSpeed = walkSpeed;

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

	GameStats gameStats;
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
			currentSpeed += 5.0f * Time::GetDelta();
			if (currentSpeed > sprintSpeed)
				currentSpeed = sprintSpeed;

			currentCameraDistance += Time::GetDelta() * 10.0f;
			if (currentCameraDistance > maxCameraDistance)
				currentCameraDistance = maxCameraDistance;
		}
			
		else
		{
			currentSpeed -= 12.0f * Time::GetDelta();
			if (currentSpeed < walkSpeed)
				currentSpeed = walkSpeed;

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
		moveDirection = moveDirection * currentSpeed * Time::GetDelta();
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
		:Model("PlayerArrow"), sceneCamera(camera)
	{
		rotation = { 0, PI, 0 };
	}

	Inventory& Inventory() { return inventory; }
	GameStats& GameStats() { return gameStats; }
};