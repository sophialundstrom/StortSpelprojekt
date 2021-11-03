#include "NPCHostile.h"


HostileNPC::HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player, CombatStyle combatStyle)
	:NPC(file)
{
	this->arrows = hostileArrows;
    this->player = player;
    this->combatStyle = combatStyle;
    SwapCombatStyle(combatStyle);
}

HostileNPC::HostileNPC(const Model& model)
	:NPC(model)
{

}

void HostileNPC::SwapCombatStyle(CombatStyle newCombatStyle)
{
    //speed(330.0f), lifeTime(3.0f)

    combatStyle = newCombatStyle;
    shootPatternIndex = 0;

    float normalDelay = 1.f;
    float quickDelay = 0.2f;
    float breakTime = 2.4f;

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

    default:
        break;
    }
}

void HostileNPC::Update()
{
     static float lastClick = 0;

     Vector3 aimDir = player->GetPosition() - position;

     std::cout << aimDir.Length() << "    " << enemyShootDetectionRadius << std::endl;

    if (aimDir.Length() <=  enemyShootDetectionRadius)
    {

        aimDir.Normalize();
        shootPatternIndex = ((shootPatternIndex++) % 3);

        movementYRadiant = acos(aimDir.x * 0 + aimDir.z * 1);
        if (aimDir.x < 0)
            movementYRadiant *= -1;

        movementXRadiant = acos(aimDir.Dot(Vector3(0, 1, 0)) / aimDir.Length());

        SetRotation({ PI_DIV2 - movementXRadiant, movementYRadiant, 0 });



        if (Time::Get() - lastClick > shootDeelayPattern[shootPatternIndex] && combatStyle != CombatStyle::wideArrow)
        {

            for (int i = 0; i < arrows.size(); i++)
            {
                if (arrows[i]->IsShot())
                    continue;

                arrows.at(i)->Shoot(aimDir, position, {PI_DIV2 - movementXRadiant, movementYRadiant, 0});
                lastClick = Time::Get();
                break;
            }
        }
        else if (Time::Get() - lastClick > 3 && combatStyle == CombatStyle::wideArrow)
        {
            float arrowWidth = PI/32.f;

            arrows.at(0)->Shoot(aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            arrows.at(1)->Shoot(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            arrows.at(2)->Shoot(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });
            
            DirectX::XMMatrixRotationX(-arrowWidth);

            lastClick = Time::Get();
           
        }



    }

    for (int i = 0; i < arrows.size(); i++)
    {
        arrows.at(i)->Update();
    }

	NPC::Update();
}

void HostileNPC::WeaponSlash()
{
	//Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
