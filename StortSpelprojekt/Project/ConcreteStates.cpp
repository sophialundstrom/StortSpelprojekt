#include "ConcreteStates.h"
#include "NPCHostile.h"

void ShootingState::SwapCombatStyle(CombatStyle newCombatStyle)
{
    combatStyle = newCombatStyle;
    shootPatternIndex = 0;

    float normalDelay = 2.f;
    float quickDelay = 0.2f;
    float breakTime = 2.4f;
    float superSlow = 864000; //86 400 seconds aka 1 real life day

    switch (combatStyle)
    {
    case CombatStyle::consistantDelay:
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

void ShootingState::Enter(HostileNPC& hostile)
{
    this->combatStyle = CombatStyle::consistantDelay;

}

void ShootingState::Update(HostileNPC& hostile)
{
    // hostile.state = &NPCState::shooting;
         // This stops multiple instances of this class from shooting at once. All instances of the same class has the same static variable... 
     // static float lastClick = 0;
    SwapCombatStyle(CombatStyle::consistantDelay);
    Vector3 aimDir = hostile.GetPlayer()->GetPosition() + Vector3(0.f, 3.5f, 0.f) - hostile.GetPosition();

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

        hostile.SetRotation({ 0, movementYRadiant, 0 });

        if (Time::Get() - lastShot > shootDeelayPattern[shootPatternIndex] && combatStyle != CombatStyle::wideArrow) // CURRENTLY THE ONLY WORKING MODE...
        {
            hostile.GetArrowHandler().AddArrow(aimDir, hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            lastShot = Time::Get();
        }
        else if (Time::Get() - lastShot > 3 && combatStyle == CombatStyle::wideArrow)
        {
            float arrowWidth = PI / 32.f;
            hostile.GetArrowHandler().AddArrow(aimDir, hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            hostile.GetArrowHandler().AddArrow(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            hostile.GetArrowHandler().AddArrow(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });

            DirectX::XMMatrixRotationX(-arrowWidth);
            lastShot = Time::Get();

        }
    }
}

void ShootingState::Exit(HostileNPC& hostile)
{
}

NPCState& ShootingState::GetInstance()
{
    static ShootingState singleton;
    return singleton;
}

//----------Moving State------------//

void MovingState::Enter(HostileNPC& hostile)
{
}

void MovingState::Update(HostileNPC& hostile)
{
}

void MovingState::Exit(HostileNPC& hostile)
{
}

NPCState& MovingState::GetInstance()
{
    static MovingState singleton;
    return singleton;
}

void IdlingState::Enter(HostileNPC& hostile)
{
}

void IdlingState::Update(HostileNPC& hostile)
{
}

void IdlingState::Exit(HostileNPC& hostile)
{
}

NPCState& IdlingState::GetInstance()
{
    static IdlingState singleton;
    return singleton;
    // TODO: insert return statement here
}