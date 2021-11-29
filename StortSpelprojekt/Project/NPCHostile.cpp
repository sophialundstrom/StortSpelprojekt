#include "NPCHostile.h"
#include "ConcreteStates.h"

HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle)
	:NPC(file)
{
    this->player = player;
    //this->combatStyle = combatStyle;
    //SwapCombatStyle(combatStyle);
 
    currentState = &ShootingState::GetInstance();
    //SetState(IdlingState::)

    arrowHandler.SetPullbackFactor(0.6f);
}

void HostileNPC::Update()
{
}

void HostileNPC::Update(const std::shared_ptr<Player> player)
{
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

        if ((arrow->GetPosition() - player->GetPosition()).Length() > 10.f)
            continue;

        if (arrowHandler.CheckCollision(arrow, player->GetBounds(), true))
        {
            player->TakeDamage();
        }
    }
}