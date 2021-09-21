#pragma once
#include "Camera.h"
#include <map>

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
	Inventory& Inventory() { return inventory; }
	GameStats& GameStats() { return gameStats; }
};