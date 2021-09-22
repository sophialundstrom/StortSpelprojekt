#pragma once
#include "Camera.h"
#include <map>
#include "Model.h"
#include "Event.h"

struct Item
{
	UINT ID;
};

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
	struct Stats
	{
		float movementSpeed;
		UINT health;
		//OSV
	} stats;


	//Player Variables and function(s)//TODO: MAKE THIS INTO A PLAYER CLASS!!!
	float movementOfsetRadiant = 0;
	float playerMoveSpeed = 4;
	float heightMapGroundLevel = 5;
	float mouseSensitivity = 10.f;
	float camDistance = 10;//How far the camera is from the playerboject

	float gravity = 9.82f;
	float timePassed = 0;

	bool jumping = false;
	bool maxJumpHeight = false;

	float jumpVelocity = 0;
	float playerVelocity = 0;
	float airTime = 0;

	float get2dAngle(Vector2 a, Vector2 b)
	{
		//MathExplanation
		//https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors

		a.Normalize();
		b.Normalize();

		return acos(a.x * b.x + a.y * b.y);
	};


	GameStats gameStats;
	Inventory inventory;
public:
	void Update(Camera* camera)
	{
		//Rotate camera by cursor movement 
		camera->Rotate(Event::ReadRawDelta().x * mouseSensitivity, Event::ReadRawDelta().y * mouseSensitivity);
		Event::ClearRawDelta();

		//Get players position last frame and cameras current look-direction

		Vector3 lookDirection = camera->GetDirection();

		//Vector that defines the direction the player move
		Vector3 moveDirection = Vector3(0, 0, 0);
		if (Event::KeyIsPressed('W'))
			moveDirection += Vector3(0, 0, 1);
		if (Event::KeyIsPressed('S'))
			moveDirection += Vector3(0, 0, -1);
		if (Event::KeyIsPressed('A'))
			moveDirection += Vector3(-1, 0, 0);
		if (Event::KeyIsPressed('D'))
			moveDirection += Vector3(1, 0, 0);
		moveDirection.Normalize();

		//Calculate playerJumpVelocity
		jumpVelocity = sqrtf(2 * gravity * 1);
		

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
		moveDirection = moveDirection * (playerMoveSpeed * Time::GetDelta());
		Vector3 newPlayerPos = position + moveDirection + Vector3(0, playerVelocity, 0);

		if (Event::KeyIsPressed(VK_SPACE))
		{
			jumping = true;
		}

		if (jumping && !Event::KeyIsPressed(VK_SPACE))
		{
			if (!maxJumpHeight)
			{
				airTime += Time::GetDelta() * 0.005f;
				playerVelocity = 0.5f * (+gravity) * powf(airTime, 2) + float(jumpVelocity) * airTime;
				if (newPlayerPos.y >= 20)
				{
					maxJumpHeight = true;
					jumping = false;
				}
			}			
		}

		if (!jumping)
		{
			if (playerVelocity >= 0)
			{
				airTime += Time::GetDelta() * 0.5f;
				playerVelocity = 0.5f * (-gravity) * powf(airTime, 2) - float(jumpVelocity) * airTime;
			}
			
			if (newPlayerPos.y <= 5)
			{
				airTime = 0;
				maxJumpHeight = false;
				jumping = false;
			}
			
		}

		if (newPlayerPos.y < heightMapGroundLevel)
		{
			newPlayerPos = Vector3(newPlayerPos.x, heightMapGroundLevel, newPlayerPos.z);
		}

		Vector3 newCameraPos = position + (lookDirection * -camDistance);
		position = newPlayerPos;
		camera->SetPosition(newCameraPos);


		std::cout << jumping << std::endl;
	};


	Player()
		:Model("PlayerArrow")
	{
		//Player
		position = { 0, 5, 0 };
		rotation = { 0, PI, 0 };
	}

	Inventory& Inventory() { return inventory; }
	GameStats& GameStats() { return gameStats; }
};