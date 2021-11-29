#include "BarbarianCamp.h"
#include "Renderers.h"

BarbarianCamp::Location BarbarianCamp::GetLocation()    { return location; }
UINT BarbarianCamp::NumBarbarians()                     { return numBarbarians; }

void BarbarianCamp::SetTarget(const Vector3& target)
{
    this->target = target;
}

void BarbarianCamp::Update(std::shared_ptr<Player> player)
{
    //NOT CORRECT BUT WORKS FOR NOW
    for (auto& barbarian : barbarians)
    {
        if (location == Location::Village)
        {
           /* if ((player->GetPosition() - barbarian->GetPosition()).Length() < barbarian->DetectionRadius())
                target = player->GetPosition();*/
        }

       // barbarian->Update(player);
    }

    if (location != Location::Village)
    {
        if ((player->GetPosition() - position).Length() < radius)
            target = player->GetPosition();
    }
}

void BarbarianCamp::Reset()
{
    for (auto& barbarian : barbarians)
        barbarian->Reset();
}

BarbarianCamp::BarbarianCamp(const Vector3& position, Location location,  float radius, bool active)
    :location(location), radius(radius), active(active), numBarbarians(0) {}

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

    if (!active)
        barbarian->Die();

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