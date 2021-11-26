#pragma once
#include "Item.h"
#include <map>

class Inventory
{
	friend class Player;
private:
	std::map<Item::Type, UINT> items;
public:
	void AddItem(Item::Type type, UINT amount = 1);
	void RemoveItem(Item::Type type, UINT amount = 1);
	UINT NumOf(Item::Type type);
};