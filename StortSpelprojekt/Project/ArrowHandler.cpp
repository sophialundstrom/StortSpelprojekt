#include "ArrowHandler.h"
#include "Renderers.h"
#include "Random.h"

void ArrowHandler::AddArrow(const Vector3& direction, const Vector3& startPos, const Vector3& rotation)
{
    std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
    arrow->SetRotation({ rotation.x, rotation.y + PI, rotation.z });
    arrow->direction = direction;
    arrow->SetPosition(startPos);
    CR->Bind(arrow->rayCollider);
    MR->Bind(arrow);
    arrows.emplace_back(arrow);
}

void ArrowHandler::Update()
{
    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i]->Update();

        if (arrows[i]->isDestroyed)
        {
            CR->Unbind(arrows[i]->rayCollider);
            MR->Unbind(arrows[i]);
            arrows[i] = std::move(arrows[arrows.size() - 1]);
            arrows.resize(arrows.size() - 1);
        }
    }
}

bool ArrowHandler::CheckCollision(std::shared_ptr<Arrow> arrow, std::shared_ptr<Collider> collider, const Vector3& playerPosition, bool isDynamic)
{
        Collision::RayResults rayResult;
        Vector3 point;

        auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
        if (box)
        {
            rayResult = Collision::Intersection(*box, *arrow->rayCollider);
            point = arrow->rayCollider->origin + arrow->rayCollider->direction * rayResult.distance;
        }

        auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
        if (sphere)
        {
            rayResult = Collision::Intersection(*sphere, *arrow->rayCollider);
            point = arrow->rayCollider->origin + arrow->rayCollider->direction * rayResult.distance;
        }

        if (rayResult.didHit)
        {
            if (isDynamic)
            {
                arrow->isDestroyed = true;
                arrow->isStuck = true;
                arrow->canCollide = false;
            }
            else
            {
                Vector3 pullBackVector = (point - arrow->GetPosition()) * this->pullbackFactor;
                arrow->SetPosition(arrow->GetPosition() + pullBackVector);
                arrow->isStuck = true;
                arrow->canCollide = false;
            }

            if ((playerPosition - arrow->GetPosition()).Length() < 50.f)
            {
                int rand = Random::Integer(0, 1);
                if (rand == 0)
                    Audio::StartEffect("ArrowHit.wav");
                else
                    Audio::StartEffect("ArrowHitWall.wav");
            }
        }
        
        return rayResult.didHit; // this creates the problem. if an arrow returns false (misses) no other will hit either. if it hits all others will hit.

    //}

    return false;
}

void ArrowHandler::ClearArrows()
{
    for (auto& arrow : arrows)
    {
        MR->Unbind(arrow);
        CR->Unbind(arrow->rayCollider);
    }
}
