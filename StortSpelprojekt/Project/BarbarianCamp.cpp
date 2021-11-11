#include "BarbarianCamp.h"

UINT BarbarianCamp::GetID() { return ID; }

UINT BarbarianCamp::NumDead()
{
    UINT counter = 0;
    for (auto& barbarian : barbarians)
        if (barbarian->IsDead())
            counter++;
    return counter;
}
