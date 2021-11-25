#include "NPCHostile.h"
#include "ConcreteStates.h"


HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
	:NPC(file)
{
    this->player = player;
   // this->currentState = &NPCState::idling;
    currentState = &ShootingState::GetInstance();
   // SetState(IdlingState::)
    mRend = &mRenderer;
    cRend = &cRenderer;

    arrowHandler.SetPullbackFactor(0.6f);
}

HostileNPC::HostileNPC(const Model& model)
	:NPC(model)
{

}

void HostileNPC::Update()
{
    currentState->Update(*this);
    arrowHandler.Update(*mRend, *cRend);

    NPC::Update();
   
}

void HostileNPC::Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const std::shared_ptr<Player> player)
{
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
            std::cout << "PLAYER HIT" << std::endl;
            player->TakeDamage();
        }
        
    }
}

void HostileNPC::WeaponSlash()
{
	// Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
