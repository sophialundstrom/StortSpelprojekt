#include "BarbarianCamp.h"
#include "Renderers.h"

CampData::Location BarbarianCamp::GetLocation()    { return location; }
UINT BarbarianCamp::NumBarbarians()                     { return numBarbarians; }

void BarbarianCamp::SetTarget(const Vector3& target)
{
    this->target = target;
}

void BarbarianCamp::Update(std::shared_ptr<Player> player, HeightMap* heightMap)
{
    //NOT CORRECT BUT WORKS FOR NOW
    for (auto& barbarian : barbarians)
    {
        if (location == CampData::Location::Village)
        {
            barbarian->Update(player, heightMap);
        }

       // barbarian->Update(player);
    }

    /*if (location != Location::Village)
    {
        if ((player->GetPosition() - position).Length() < radius)
            target = player->GetPosition();
    }*/
}

void BarbarianCamp::Reset()
{
    for (auto& barbarian : barbarians)
        barbarian->Reset();
}

BarbarianCamp::BarbarianCamp(const Vector3& position, CampData::Location location,  float radius, bool active)
    :location(location), radius(radius), active(active), numBarbarians(0) 
{}

void BarbarianCamp::AddBarbarian(const std::string& file, const Vector3& position, std::vector<std::shared_ptr<HostileNPC>>& hostiles, std::shared_ptr<Player> player, CombatStyle combatStyle, const Vector3& targetPosition, std::shared_ptr<Pathfinding> pathing, bool dynamic, int damage, int health, bool moving)
{
    auto barbarian = std::make_shared<HostileNPC>(file, player, combatStyle, targetPosition, pathing, health, moving);
    barbarian->SetPosition(position);
    barbarian->SetDamage(damage);
   // auto barbarian = std::make_shared<HostileNPC>(file, position, player, combatStyle, targetPosition, );
   // barbarian->SetPosition(position);

    barbarians.emplace_back(barbarian);
    hostiles.emplace_back(barbarian);

    //MR->Bind(barbarian);
    AMR->Bind(barbarian);
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