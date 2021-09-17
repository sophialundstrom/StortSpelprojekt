#pragma once
#include "Camera.h"

struct Inventory { Inventory() = default; };

struct GameStats
{
	//MOSTLY FOR QUEST PROGRESS BUT MAYBE FUN TO KNOW WHEN GAME IS OVER?
	UINT barbariansKilled = 0;
};

class Player
{
private:
	struct Stats
	{
		float movementSpeed;
		UINT health;
		//OSV
	} stats;

	GameStats gameStats;
	Inventory inventory;
public:
	Player() = default;
};