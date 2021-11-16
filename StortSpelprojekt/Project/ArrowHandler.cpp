#include "ArrowHandler.h"

void ArrowHandler::AddArrow(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, const Vector3& startPos, const Vector3& rotation)
{
    std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
    arrow->SetRotation({ rotation.x, rotation.y + PI, rotation.z });
    arrow->direction = direction;
    arrow->SetPosition(startPos);
    arrow->isShot = true;
    arrow->GetCollider()->SetParent(arrow);
    arrow->GetCollider()->SetScale(0.4f);
    arrow->GetCollider()->SetPosition(arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z - 0.5f);
    mRenderer.Bind(arrow);
    cRenderer.Bind(arrow->GetCollider());
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
        if (box)
            hit = Collision::Intersection(box, arrow->GetCollider());

        auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
        if (sphere)
            hit = Collision::Intersection(sphere, arrow->GetCollider());

        if (hit)
        {
            if (isDynamic)
            {
                arrow->isDestroyed = true;
                std::cout << "HIT on dynamic object" << std::endl;
            }
            else
            {
                std::cout << "HIT on static object" << std::endl;
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
        cRenderer.Unbind(arrow);

    }
}
