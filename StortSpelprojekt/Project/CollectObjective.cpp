#include "CollectObjective.h"

void CollectObjective::Update(Inventory& inventory)
{
    numCollected = inventory.NumOf(type);

    if (numCollected >= numToCollect)
        completed = true;
}

std::string CollectObjective::Info()
{
    return "Collect " + std::to_string(numToCollect - numCollected) + " " + Item::Names[(UINT)type];
}