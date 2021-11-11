#pragma once
#include "Item.h"
#include <map>

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

	UINT NumOf(Item::Type type)
	{
		return items[(UINT)type];
	}
};