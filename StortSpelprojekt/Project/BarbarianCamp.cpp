#include "BarbarianCamp.h"

BarbarianCamp::Location BarbarianCamp::GetLocation()    { return location; }
UINT BarbarianCamp::NumBarbarians()                     { return numBarbarians; }

BarbarianCamp::BarbarianCamp(Location location, UINT numBarbarians, float radius)
    :location(location), numBarbarians(numBarbarians)
{
    barbarians = new std::shared_ptr<HostileNPC>[numBarbarians];
}

BarbarianCamp::~BarbarianCamp()
{
    delete[] barbarians;
}

UINT BarbarianCamp::NumDead()
{
    UINT counter = 0;
    for (UINT i = 0; i < numBarbarians; ++i)
        if (barbarians[i]->IsDead())
            counter++;
    return counter;
}
