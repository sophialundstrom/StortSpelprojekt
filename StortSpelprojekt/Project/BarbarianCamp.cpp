#include "BarbarianCamp.h"
#include "Renderers.h"

BarbarianCamp::Location BarbarianCamp::GetLocation()    { return location; }
UINT BarbarianCamp::NumBarbarians()                     { return numBarbarians; }

void BarbarianCamp::Reset()
{
    for (auto& barbarian : barbarians)
        barbarian->Reset();
}

BarbarianCamp::BarbarianCamp(Location location,  float radius)
    :location(location), radius(radius), numBarbarians(0) {}

void BarbarianCamp::AddBarbarian(const std::string& file, const Vector3& position, std::vector<std::shared_ptr<HostileNPC>>& hostiles, std::shared_ptr<Player> player, CombatStyle combatStyle, bool dynamic)
{
    auto barbarian = std::make_shared<HostileNPC>(file, player, combatStyle);
    barbarian->SetPosition(position);
    barbarians.emplace_back(barbarian);
    hostiles.emplace_back(barbarian);
    MR->Bind(barbarian);
    SR->Bind(barbarian);

    auto collider = barbarian->GetCollider();
    collider->SetParent(barbarian);
    collider->Update();
    collider->SetScale(2, 7, 2);
    CR->Bind(collider);

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