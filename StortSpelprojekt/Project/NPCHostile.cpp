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

HostileNPC::HostileNPC(const Model& model)
	:NPC(model)
{

}

void HostileNPC::Update()
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

        if (arrowHandler.CheckCollision(arrow, player->GetBounds(), true))
        {
            player->TakeDamage();
        }
    }
}

void HostileNPC::WeaponSlash()
{
	// Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
