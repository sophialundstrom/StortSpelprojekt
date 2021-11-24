#include "ArrowHandler.h"

void ArrowHandler::AddArrow(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, const Vector3& startPos, const Vector3& rotation)
{
    std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
    arrow->SetRotation({ rotation.x, rotation.y + PI, rotation.z });
    arrow->direction = direction;
    arrow->SetPosition(startPos);
    cRenderer.Bind(arrow->rayCollider);
    mRenderer.Bind(arrow);
    arrows.emplace_back(arrow);
}

void ArrowHandler::Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
{
    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i]->Update();
        //if (!arrows[i]->canCollide)
        //    PrintS("CANT COLLIDE!");

        if (arrows[i]->isDestroyed)
        {
            cRenderer.Unbind(arrows[i]->rayCollider);
            mRenderer.Unbind(arrows[i]);
            //arrows.erase(arrows.begin() + i);
            arrows[i] = std::move(arrows[arrows.size() - 1]);
            arrows.resize(arrows.size() - 1);
        }
    }
}

bool ArrowHandler::CheckCollision(std::shared_ptr<Arrow> arrow, std::shared_ptr<Collider> collider, bool isDynamic)
{
    /*for (auto& arrow : arrows)
    {*/
        //if (!arrow->canCollide)
        //    continue;

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
            //SoundEffect::AddAudio(L"Audio/arrowHit.wav", 2);
            //SoundEffect::SetVolume(0.8, 2);
            //SoundEffect::StartAudio(2);
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
        }
        
        return rayResult.didHit; // this creates the problem. if an arrow returns false (misses) no other will hit either. if it hits all others will hit.

    //}

    return false;
}

void ArrowHandler::ClearArrows(ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
{
    for (auto& arrow : arrows)
    {
        mRenderer.Unbind(arrow);
        cRenderer.Unbind(arrow->rayCollider);
    }
}
