#include "ArrowHandler.h"

void ArrowHandler::AddArrow(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, const Vector3& startPos, const Vector3& rotation)
{
    std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
    arrow->SetRotation({ rotation.x, rotation.y + PI, rotation.z });
    arrow->direction = direction;
    arrow->SetPosition(startPos);
    arrow->isShot = true;
    //arrow->GetCollider()->SetParent(arrow);
    //arrow->GetCollider()->SetScale(0.4f);
    //arrow->GetCollider()->SetPosition(arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z - 0.5f);
    cRenderer.Bind(arrow->rayCollider);
    mRenderer.Bind(arrow);
    //cRenderer.Bind(arrow->GetCollider());
    arrows.emplace_back(arrow);
}

void ArrowHandler::Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
{
    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i]->Update();

        if (arrows[i]->isDestroyed)
        {
            std::cout << "Arrow destroyed!" << std::endl;
            cRenderer.Unbind(arrows[i]->GetCollider());
            cRenderer.Unbind(arrows[i]->rayCollider);
            mRenderer.Unbind(arrows[i]);
            arrows[i] = arrows[arrows.size() - 1];
            arrows.resize(arrows.size() - 1);
        }
    }
}

bool ArrowHandler::CheckCollision(std::shared_ptr<Collider> collider, bool isDynamic)
{
    for (auto& arrow : arrows)
    {
        if (!arrow->canCollide)
            continue;

        bool hit = false;

        auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
        Collision::RayResults rayResult;
        Vector3 point;
        if (box)
        {
            rayResult = Collision::Intersection(*box, *arrow->rayCollider);

            point = arrow->rayCollider->origin + arrow->rayCollider->direction * rayResult.distance;

            //if (Collision::Contains(*box, point))
            //{
            //    Vector3 temp = arrow->rayCollider->origin + arrow->rayCollider->direction * arrow->rayCollider->length;
            //    //std::cout << "BOX CONTAINS: " << temp.x << " " << temp.y << " " << temp.z << std::endl;
            //    hit = true;
            //}
        }

        auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
        if (sphere)
        {
            rayResult = Collision::Intersection(*sphere, *arrow->rayCollider);
            point = arrow->rayCollider->origin + arrow->rayCollider->direction * rayResult.distance;
            //if (Collision::Contains(*sphere, point))
            //{
            //    Vector3 temp = arrow->rayCollider->origin + arrow->rayCollider->direction * arrow->rayCollider->length;
            //    //std::cout << "BOX CONTAINS: " << temp.x << " " << temp.y << " " << temp.z << std::endl;
            //    hit = true;
            //}

        }

        if (rayResult.didHit)
        {
            if (isDynamic)
            {
                arrow->isDestroyed = true;
                std::cout << "HIT on dynamic object" << std::endl;
            }
            else
            {
                std::cout << "DISTANCE THIS FRAME: " << rayResult.distance << std::endl;
                Vector3 pullBackVector = (point - arrow->GetPosition()) * this->pullbackFactor;
                //std::cout << "POINT: " << point.x << " " << point.y << " " << point.z << std::endl;
                //std::cout << "ARROW POS: " << arrow->GetPosition().x << " " << arrow->GetPosition().y << " " << arrow->GetPosition().z << std::endl;
                std::cout << "PULLBACK: " << pullBackVector.x << " " << pullBackVector.y << " " << pullBackVector.z << std::endl;
                arrow->SetPosition(arrow->GetPosition() + pullBackVector);
                arrow->isStuck = true;
                arrow->canCollide = false;
            }
        }
        return hit;


    }

    return false;
}

void ArrowHandler::ClearArrows(ModelRenderer& mRenderer, ColliderRenderer& cRenderer)
{
    for (auto& arrow : arrows)
    {
        mRenderer.Unbind(arrow);
        cRenderer.Unbind(arrow->GetCollider());
        cRenderer.Unbind(arrow->rayCollider);

    }
}
