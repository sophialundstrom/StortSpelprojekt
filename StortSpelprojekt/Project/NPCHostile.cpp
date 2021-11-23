#include "NPCHostile.h"



HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
	:NPC(file)
{
    this->player = player;
    this->state = NPCState
    this->combatStyle = combatStyle;
    SwapCombatStyle(combatStyle);
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
    state->update(*this);
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
