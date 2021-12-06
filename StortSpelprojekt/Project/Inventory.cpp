#include "Inventory.h"

void Inventory::AddItem(Item::Type type, UINT amount)
{
	items[type] += amount;
}

void Inventory::RemoveItem(Item::Type type, UINT amount)
{
	if (items[type] <= amount)
	{
		items[type] = 0;
		return;
	}

	items[type] -= amount;
}

UINT Inventory::NumOf(Item::Type type)
{
	return items[type];
}