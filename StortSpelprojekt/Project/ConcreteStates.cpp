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

    if ((hostile.GetPosition() - hostile.targetPosition).Length() >= 3.f && hostile.distanceToPlayer > hostile.viewDistance)
    {
        PrintS("CHANGED FROM SHOOT -> MOVING");
        hostile.currentState = &MovingState::GetInstance();
        hostile.SetRotation(hostile.originalRotation);
        return;
    }
    if ((hostile.GetPosition() - hostile.targetPosition).Length() < 3.f && hostile.distanceToPlayer > hostile.viewDistance)
    {
        PrintS("CHANGED FROM SHOOT -> MOVING");
        hostile.currentState = &IdlingState::GetInstance();
        return;
    } 

    SwapCombatStyle(CombatStyle::consistantDelay);
    Vector3 aimDir = hostile.GetPlayer()->GetPosition() - hostile.GetPosition();

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

        if (Time::Get() - hostile.lastShot > shootDeelayPattern[shootPatternIndex] && combatStyle != CombatStyle::wideArrow) // CURRENTLY THE ONLY WORKING MODE...
        {
            hostile.GetArrowHandler().AddArrow(aimDir, hostile.GetPosition() + Vector3(0,6,0), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            Audio::StartEffect("Fire.wav");
            PrintS("SHOT");
            hostile.lastShot = Time::Get();
        }
        else if (Time::Get() - hostile.lastShot > 3 && combatStyle == CombatStyle::wideArrow)
        {
            float arrowWidth = PI / 32.f;
            Audio::StartEffect("Fire.wav");
            hostile.GetArrowHandler().AddArrow(aimDir, hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            hostile.GetArrowHandler().AddArrow(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            hostile.GetArrowHandler().AddArrow(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), hostile.GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });

            DirectX::XMMatrixRotationX(-arrowWidth);
            hostile.lastShot = Time::Get();

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
    Node* node = hostile.GetPath()->GetClosestNode(hostile.GetPosition(), 1000);
    if (hostile.GetPath()->GetClosestNode(hostile.GetPosition(), 1000) != nullptr)
    {
        hostile.GetPath()->FindPath(hostile.GetPath()->GetClosestNode(hostile.GetPosition(), 1000)->position, hostile.GetPlayer()->GetPosition());
    }
    else
    {
        hostile.SetState(IdlingState::GetInstance());
        //delete node;
        return;
    }

    hostile.path = hostile.GetPath()->GetGrid()->GetPathRef();
    for (int i = 0; i < hostile.GetPath()->GetGrid()->GetIDs().size(); i++)
    {
        if (node->id == hostile.GetPath()->GetGrid()->GetIDs()[i])
            hostile.pathIndex = i;
    }
    //hostile.pathIndex = node->id;
}

void MovingState::Update(HostileNPC& hostile)
{
    //TODO: Implement walking behaviour
    Vector3 moveDirection;
    Vector3 aimDir;// = hostile.GetPlayer()->GetPosition() + Vector3(0.f, 4.5f, 0.f) - hostile.GetPosition();

    if (Vector3::Distance(hostile.GetPosition(), hostile.GetPlayer()->GetPosition()) > 40.0f)
    {

	    if (hostile.pathIndex < hostile.path.size())
	    {
		    moveDirection = hostile.path[hostile.pathIndex] - hostile.GetPosition();
		    moveDirection.Normalize();
            aimDir = moveDirection;

            hostile.GetPositionRef() += (moveDirection * speed * Time::GetDelta());

		    if (Vector3::Distance(hostile.path[hostile.pathIndex], hostile.GetPosition()) < 5.2f)
                hostile.pathIndex++;
	    }
	    else
	    {
            // find a new path here
            hostile.path.clear();
            hostile.SetState(IdlingState::GetInstance());
		    return;
	    }
    }
    else // delete path because we want to stop before collision 
    {
	    hostile.path.clear();
        hostile.SetState(IdlingState::GetInstance());
        return;
    }
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
    float movementYRadiant = additionalRadians + acos(aimDir.Dot(yRadiantVecReference) / aimDir.Length());
    //movementXRadiant = acos(aimDir.Dot(Vector3(0, 1, 0)) / aimDir.Length());

    hostile.SetRotation({ 0, movementYRadiant, 0 });

    

    //Vector3 direction = hostile.targetPosition - hostile.GetPosition();
    //direction.Normalize();

    //Vector3 newPosition = hostile.GetPosition() + direction * (hostile.speed * Time::GetDelta());
    //if(hostile.isMoving)
    //    newPosition.y = hostile.heightMapGroundLevel;

    if (Event::KeyIsPressed('7'))
    {
        PrintNumber(hostile.viewDistance, "VIEW DIST: ");
        PrintNumber(hostile.distanceToPlayer, "DIST TO PLAYER: ");
    }
    if (hostile.distanceToPlayer < hostile.viewDistance)
    {
        PrintS("CHANGED FROM MOVING -> SHOOT");
        Audio::StartEffect("BarbNoticed.wav");
        hostile.currentState = &ShootingState::GetInstance();
    }
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
    if (hostile.distanceToPlayer < hostile.viewDistance )
    {
        PrintS("CHANGED FROM IDLE -> SHOOT");
        Audio::StartEffect("BarbNoticed.wav");
        hostile.currentState = &ShootingState::GetInstance();
        return;
    }
    if (hostile.distanceToPlayer > hostile.viewDistance && (hostile.GetPosition() - hostile.targetPosition).Length() > 3.f)
    {
        PrintS("CHANGED FROM IDLE -> MOVING");
        hostile.currentState = &MovingState::GetInstance();
        hostile.SetRotation(hostile.originalRotation);
        return;
    }
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