#include "BarbarianCamp.h"

BarbarianCamp::Location BarbarianCamp::GetLocation()    { return location; }
UINT BarbarianCamp::NumBarbarians()                     { return numBarbarians; }

BarbarianCamp::BarbarianCamp(Location location,  float radius)
    :location(location), radius(radius), numBarbarians(0) {}

void BarbarianCamp::AddBarbarian(const std::string& file, const Vector3& position, bool dynamic)
{
    auto barbarian = std::make_shared<HostileNPC>(file, Locations[UINT(location)] + "_Barbarian_" + std::to_string(numBarbarians));
    barbarians.emplace_back(barbarian);
    numBarbarians++;
}

UINT BarbarianCamp::NumDead()
{
    UINT counter = 0;
    for (UINT i = 0; i < numBarbarians; ++i)
        if (barbarians[i]->IsDead())
            counter++;
    return counter;
}
