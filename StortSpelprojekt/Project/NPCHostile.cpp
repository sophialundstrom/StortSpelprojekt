#include "NPCHostile.h"

HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
	:NPC(file)
{
    this->player = player;
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

void HostileNPC::SwapCombatStyle(CombatStyle newCombatStyle)
{
    combatStyle = newCombatStyle;
    shootPatternIndex = 0;

    float normalDelay = 2.f;
    float quickDelay = 0.2f;
    float breakTime = 2.4f;
    float superSlow = 864000; //86 400 seconds aka 1 real life day

    switch (combatStyle)
    {
    case CombatStyle::consistantDelay :
        shootDeelayPattern[0] = normalDelay;
        shootDeelayPattern[1] = normalDelay;
        shootDeelayPattern[2] = normalDelay;
        break;

    case CombatStyle::Burst:
        shootDeelayPattern[0] = quickDelay;
        shootDeelayPattern[1] = quickDelay;
        shootDeelayPattern[2] = quickDelay + breakTime;
        break;

    case CombatStyle::Defenseless:
        shootDeelayPattern[0] = superSlow;
        shootDeelayPattern[1] = superSlow;
        shootDeelayPattern[2] = superSlow;
        break;

    default:
        break;
    }
}

void HostileNPC::Update()
{
   
}

void HostileNPC::Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const std::shared_ptr<Player> player)
{
    // This stops multiple instances of this class from shooting at once. All instances of the same class has the same static variable... 
    // static float lastClick = 0;

    Vector3 aimDir = player->GetPosition() + Vector3(0.f,3.5f,0.f) - position;

    if (aimDir.Length() <= enemyShootDetectionRadius)
    {

        aimDir.Normalize();
        shootPatternIndex = ((shootPatternIndex++) % 3);
        float additionalRadians = 0;

        Vector3 yRadiantVecReference;

        float aimDirXIgnoranceLevel = 0.2f;

        if (aimDir.x > -aimDirXIgnoranceLevel && aimDir.x < aimDirXIgnoranceLevel)
        {
            if (aimDir.z < 0)
            {
                yRadiantVecReference = { 1, 0, 0 };
                additionalRadians = PI_DIV2;
            }
            else if (aimDir.z > 0)
            {
                yRadiantVecReference = { -1, 0, 0 };
                additionalRadians = -PI_DIV2;
            }
        }
        else if (aimDir.x > 0)
        {
            yRadiantVecReference = { 0, 0, 1 };
            additionalRadians = 0;
        }
        else
        {
            yRadiantVecReference = { 0, 0, -1 };
            additionalRadians = PI;
        }
        movementYRadiant = additionalRadians + acos(aimDir.Dot(yRadiantVecReference) / aimDir.Length());
        movementXRadiant = acos(aimDir.Dot(Vector3(0, 1, 0)) / aimDir.Length());

        SetRotation({ 0, movementYRadiant, 0 });

        if (Time::Get() - lastShot > shootDeelayPattern[shootPatternIndex] && combatStyle != CombatStyle::wideArrow) // CURRENTLY THE ONLY WORKING MODE...
        {
            arrowHandler.AddArrow(mRenderer, cRenderer, aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            lastShot = Time::Get();
        }
        else if (Time::Get() - lastShot > 3 && combatStyle == CombatStyle::wideArrow)
        {
            float arrowWidth = PI / 32.f;
            arrowHandler.AddArrow(mRenderer, cRenderer, aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            arrowHandler.AddArrow(mRenderer, cRenderer, DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            arrowHandler.AddArrow(mRenderer, cRenderer, DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });

            DirectX::XMMatrixRotationX(-arrowWidth);
            lastShot = Time::Get();

        }

    }

    arrowHandler.Update(mRenderer, cRenderer);

    NPC::Update();
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
