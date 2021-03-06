#include "CollectObjective.h"

CollectObjective::CollectObjective(Item::Type type, UINT amount)
    :Objective(Type::COLLECT, false), itemType(type), numToCollect(amount), numCollected(0) {}

void CollectObjective::Update(Inventory& inventory)
{
    numCollected = inventory.NumOf(itemType);

    if (numCollected >= numToCollect)
        Complete();
}

std::string CollectObjective::Info()
{
    if (numToCollect == 1)
        return "Collect A " + Item::Names[(UINT)itemType];

    return "Collect " + std::to_string(numToCollect - numCollected) + " " + Item::Names[(UINT)itemType] + "s";
}

void CollectObjective::WriteToFile(File& file)
{
    Objective::WriteToFile(file);

    file.Write(numCollected);
    file.Write(numToCollect);
    file.Write(itemType);
}

void CollectObjective::ReadFromFile(File& file)
{
    this->type = Objective::Type::COLLECT;

    Objective::ReadFromFile(file);

    file.Read(numCollected);
    file.Read(numToCollect);
    file.Read(itemType);
}