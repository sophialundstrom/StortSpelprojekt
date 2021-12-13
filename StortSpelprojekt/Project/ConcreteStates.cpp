#include "ConcreteStates.h"
#include "NPCHostile.h"

void ShootingState::SwapCombatStyle(CombatStyle newCombatStyle)
{
    combatStyle = newCombatStyle;
    shootPatternIndex = 0;

    float normalDelay = 2.f;
    float quickDelay = 0.2f;
    float breakTime = 2.4f;
    float superSlow = 864000;

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

//----------Moving State------------//

void MovingState::Enter(HostileNPC& hostile)
{
    Node* startNode = hostile.GetPath()->GetClosestNode(hostile.GetPosition(), 1000);
    Node* endNode = hostile.GetPath()->GetClosestNode(hostile.startPos, 140);

    if (startNode && endNode)
    {
        hostile.GetPath()->FindPath(startNode->position, endNode->position);
        std::cout << "found path\n";
    }
    else
    {
        Print("Moving back, Node nullptr: Going to IDLING");
        hostile.SetState(IdlingState::GetInstance());
        return;
    }
    hostile.path = hostile.GetPath()->GetGrid()->GetPathRef();
    for (int i = 0; i < hostile.GetPath()->GetGrid()->GetIDs().size(); i++)
    {
        if (startNode->id == hostile.GetPath()->GetGrid()->GetIDs()[i])
            hostile.pathIndex = i;
    }
}

void MovingState::Update(HostileNPC& hostile)
{
    //TODO: Implement walking behaviour
    Vector3 moveDirection;
    Vector3 aimDir;

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
            
        hostile.path.clear();
        hostile.SetState(IdlingState::GetInstance());
        std::cout << "MOVING -> IDLING\n";
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

    hostile.SetRotation({ 0, movementYRadiant, 0 });

    if (Event::KeyIsPressed('7'))
    {
        PrintNumber(hostile.viewDistance, "VIEW DIST: ");
        PrintNumber(hostile.distanceToPlayer, "DIST TO PLAYER: ");
    }
    if (hostile.distanceToPlayer < 61)
    {
        PrintS("CHANGED FROM MOVING -> SHOOT");
        Audio::StartEffect("BarbNoticed.wav");
        hostile.currentState = &ShootingState::GetInstance();
    }
}

void MovingState::Exit(HostileNPC& hostile)
{
    hostile.path.clear();
}

//--------------Hunting state-------------------//

void HuntingState::Enter(HostileNPC& hostile)
{
    Node* startNode = hostile.GetPath()->GetClosestNode(hostile.GetPosition(), 1000);
    if (Vector3::Distance(hostile.GetPosition(), hostile.GetPlayer()->GetPosition()) > 20.0f)
    {
        Node* endNode = hostile.GetPath()->GetClosestNode(hostile.GetPlayer()->GetPosition(), 200);
        if (startNode && endNode)
        {
            hostile.GetPath()->FindPath(startNode->position, endNode->position);
            std::cout << "found path\n";
        }
        else
        {
            if (Vector3::Distance(hostile.assignedCampPos, hostile.GetPlayer()->GetPosition()) > hostile.campCutOff)//if player is far away from camp go back to default behaviour
            {
                Print("player too far away: HUNTING -> MOVEING");
                hostile.SetState(MovingState::GetInstance());
                return;
            }
            Print("HUNTING -> IDLING");
            hostile.SetState(IdlingState::GetInstance());
            return;
        }
    }
    hostile.path = hostile.GetPath()->GetGrid()->GetPathRef();
    for (int i = 0; i < hostile.GetPath()->GetGrid()->GetIDs().size(); i++)
    {
        if (startNode->id == hostile.GetPath()->GetGrid()->GetIDs()[i])
            hostile.pathIndex = i;
    }
    //hostile.pathIndex = node->id;
}

void HuntingState::Update(HostileNPC& hostile)
{
    if (Vector3::Distance(hostile.GetPosition(), hostile.assignedCampPos) > hostile.campCutOff)
    {
        hostile.SetState(ShootingState::GetInstance());
        if (hostile.distanceToPlayer > hostile.viewDistance)
        {
            hostile.SetState(MovingState::GetInstance());
        }
        return;
    }


    Vector3 moveDirection;
    Vector3 aimDir;// = hostile.GetPlayer()->GetPosition() + Vector3(0.f, 4.5f, 0.f) - hostile.GetPosition();

    if (Vector3::Distance(hostile.GetPosition(), hostile.GetPlayer()->GetPosition()) > hostile.viewDistance/1.4f)
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
            PrintS("at target node, checking around: SHOOTING");
            hostile.SetState(ShootingState::GetInstance());
            return;
        }
    }
    else // delete path because we want to stop before collision 
    {
        Print("too close: Shooting player");
        hostile.path.clear();
        hostile.SetState(ShootingState::GetInstance());
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
}

void HuntingState::Exit(HostileNPC& hostile)
{
    hostile.path.clear();
}

//-----------------SHOOTING STATE------------------//

void ShootingState::Enter(HostileNPC& hostile)
{
    this->combatStyle = CombatStyle::consistantDelay;
}

void ShootingState::Update(HostileNPC& hostile)
{
    if (hostile.distanceToPlayer > hostile.viewDistance)
    {
        PrintS("CHANGED FROM SHOOT -> HUNTING");
        hostile.SetState(HuntingState::GetInstance());
        hostile.SetRotation(hostile.originalRotation);
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
    }
}

void ShootingState::Exit(HostileNPC& hostile)
{
}

//----------------IDLING STATE----------------//

void IdlingState::Enter(HostileNPC& hostile)
{
    hostile.path.clear();
}

void IdlingState::Update(HostileNPC& hostile)
{

    if (Vector3::Distance(hostile.GetPlayer()->GetPosition(), hostile.assignedCampPos) < hostile.campCutOff + 40)
    {
        PrintS("CHANGED FROM IDLE -> MOVING");
        // hostile.currentState = &MovingState::GetInstance();
         //hostile.SetRotation(hostile.originalRotation);
        hostile.SetState(HuntingState::GetInstance());
        return;
    }
    if (hostile.distanceToPlayer < hostile.viewDistance - 30.0f )
    {
        PrintS("CHANGED FROM IDLE -> SHOOT");
        Audio::StartEffect("BarbNoticed.wav");
        hostile.SetState(ShootingState::GetInstance());
        //hostile.currentState = &ShootingState::GetInstance();
        return;
    }
}

void IdlingState::Exit(HostileNPC& hostile)
{
}

//------------GUARD STATE------------//

void GuardState::Enter(HostileNPC& hostile)
{
    this->combatStyle = CombatStyle::consistantDelay;

}

void GuardState::Update(HostileNPC& hostile)
{
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
            hostile.GetArrowHandler().AddArrow(aimDir, hostile.GetPosition() + Vector3(0, 6, 0), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            Audio::StartEffect("Fire.wav");
            PrintS("SHOT");
            hostile.lastShot = Time::Get();
        }
    }
}

void GuardState::Exit(HostileNPC& hostile)
{
}

// GetInstance of states

NPCState& IdlingState::GetInstance(){
    static IdlingState singleton;
    return singleton;
}

NPCState& MovingState::GetInstance(){
    static MovingState singleton;
    return singleton;
}

NPCState& ShootingState::GetInstance(){
    static ShootingState singleton;
    return singleton;
}

NPCState& HuntingState::GetInstance(){
    static HuntingState singleton;
    return singleton;
}

NPCState& GuardState::GetInstance(){
    static GuardState singleton;
    return singleton;
}
