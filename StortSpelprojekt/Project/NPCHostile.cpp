#include "NPCHostile.h"


void HostileNPC::Shoot(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, Vector3 startPos, Vector3 rotation)
{
    /*std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
    arrow->SetRotation({ rotation.x, rotation.y + PI, rotation.z });
    arrow->direction = direction;
    arrow->SetPosition(startPos);
    arrow->GetCollider()->SetScale(0.4f, 0.4f, 1.f);
    arrow->GetCollider()->SetPosition(arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z - 0.5f);
    arrow->GetCollider()->SetParent(arrow);
    mRenderer.Bind(arrow);
    cRenderer.Bind(arrow->GetCollider());
    arrows.emplace_back(arrow);*/
    arrowHandler.AddArrow(mRenderer, cRenderer, direction, startPos, { rotation.x, rotation.y + PI, rotation.z });
}

HostileNPC::HostileNPC(const std::string& file, std::shared_ptr<Player> player, CombatStyle combatStyle, ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
	:NPC(file)
{
    this->player = player;
    this->combatStyle = combatStyle;
    SwapCombatStyle(combatStyle);
    mRend = &mRenderer;
    cRend = &cRenderer;

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

void HostileNPC::Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const std::shared_ptr<Player> player, bool test)
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
            if (test)
            {
                std::cout << "";
            }
            arrowHandler.AddArrow(mRenderer, cRenderer, aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            //Shoot(mRenderer, cRenderer, aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            lastShot = Time::Get();
        }
        else if (Time::Get() - lastShot > 3 && combatStyle == CombatStyle::wideArrow)
        {
            float arrowWidth = PI / 32.f;
            //arrows.at(0)->Shoot(aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            //arrows.at(1)->Shoot(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            //arrows.at(2)->Shoot(DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });
            arrowHandler.AddArrow(mRenderer, cRenderer, aimDir, position, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
            arrowHandler.AddArrow(mRenderer, cRenderer, DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant + arrowWidth, 0 });
            arrowHandler.AddArrow(mRenderer, cRenderer, DirectX::XMVector3Transform(aimDir, DirectX::XMMatrixRotationY(-arrowWidth)), position, { PI_DIV2 - movementXRadiant, movementYRadiant - arrowWidth, 0 });

            DirectX::XMMatrixRotationX(-arrowWidth);
            lastShot = Time::Get();

        }

    }

    // ArrowHandle class which consists of the following:
        // vector of shared_ptr arrows
        // 
        // A update function that takes in cRenderer & mRenderer and does what is shown below (row 147 - 159).
        // This function is called inside hostile/player update.
        // 
        // The class also has a CheckCollision function which is the same as the CheckArrowHit function. This is used in game.cpp instead of the hostile/player function.
        // 
        // Also stop the overidden update function to be called each frame (if it is).

    /*for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i]->Update();

        if (arrows[i]->isDestroyed)
        {
            std::cout << "Arrow destroyed!" << std::endl;
            cRenderer.Unbind(arrows[i]->GetCollider());
            mRenderer.Unbind(arrows[i]);
            arrows[i] = arrows[arrows.size() - 1];
            arrows.resize(arrows.size() - 1);
        }
    }*/

    arrowHandler.Update(mRenderer, cRenderer);

    NPC::Update();
}

void HostileNPC::CheckPlayerCollision(std::shared_ptr<Player> player)
{
    if (arrowHandler.CheckCollision(player->GetBounds(), true))
    {
        std::cout << "PLAYER HIT" << std::endl;
        player->TakeDamage(2);
    }
}

//bool HostileNPC::CheckArrowHit(std::shared_ptr<Collider> collider, bool isDynamic)
//{
//    for (auto& arrow : arrows)
//    {
//        if (!arrow->canCollide)
//            continue;
//
//        bool hit = false;
//
//        auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
//        if (box)
//            hit = Collision::Intersection(box, arrow->GetCollider());
//
//        auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
//        if (sphere)
//            hit = Collision::Intersection(sphere, arrow->GetCollider());
//
//        if (hit)
//        {
//            if (isDynamic)
//            {
//                arrow->isDestroyed = true;
//                std::cout << "HIT on dynamic object" << std::endl;
//            }
//            else
//            {
//                std::cout << "HIT on static object" << std::endl;
//                arrow->isStuck = true;
//                arrow->canCollide = false;
//            }
//        }
//        return hit;
//
//    }
//
//    return false;
//}

void HostileNPC::WeaponSlash()
{
	// Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
