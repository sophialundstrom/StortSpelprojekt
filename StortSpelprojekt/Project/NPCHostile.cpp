#include "NPCHostile.h"
#include "ConcreteStates.h"

HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, const Vector3& targetPosition)
	:NPC(file)
{
    this->player = player;
    //this->combatStyle = combatStyle;
    //SwapCombatStyle(combatStyle);
 
    currentState = &MovingState::GetInstance();
    this->targetPosition = targetPosition;
    this->viewDistance = 80.f;
    //SetState(IdlingState::)

    arrowHandler.SetPullbackFactor(0.6f);
}

void HostileNPC::CalcHeight(HeightMap* heightMap)
{
    const int lowX = (int)std::floor(position.x);
    const int highX = (int)std::ceil(position.x);
    const float Xdecimal = position.x - lowX;

    const int lowZ = (int)std::floor(position.z);
    const int highZ = (int)std::ceil(position.z);
    const float Zdecimal = position.z - lowZ;

    const float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
    const float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
    const float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
    const float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

    heightMapGroundLevel = position.y = H1 + H2 + H3 + H4 + 6.f;
}

void HostileNPC::Update()
{
}

void HostileNPC::Update(const std::shared_ptr<Player> player, HeightMap* heightMap)
{

    CalcHeight(heightMap);
    distanceToPlayer = (player->GetPosition() - position).Length();
    currentState->Update(*this);
    arrowHandler.Update();

    NPC::Update();
 }

void HostileNPC::SetSpawnPosition(const Vector3& position)
{
    //this->spawnPosition = position;
}

void HostileNPC::Reset()
{
    //position = spawnPosition;
    dead = false;
    hp = maxHP;
}

void HostileNPC::CheckPlayerCollision(std::shared_ptr<Player> player)
{
    for (auto& arrow : arrowHandler.arrows)
    {
        if (!arrow->canCollide)
            continue;

        if ((arrow->GetPosition() - player->GetPosition()).Length() > 50.f)
            continue;

        if (arrowHandler.CheckCollision(arrow, player->GetBounds(), player->GetPosition(), true))
        {
            player->TakeDamage();
        }
    }
}