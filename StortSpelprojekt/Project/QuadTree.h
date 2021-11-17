#pragma once
#include "Math.h"
#include "Model.h"
#include "Camera.h"

struct FrustrumCollider
{
    DirectX::BoundingFrustum bounds;

    void Update(Camera camera)
    {
        
        
        bounds.CreateFromMatrix(bounds, camera.GetMatrix().Transpose());
        //bounds = DirectX::BoundingFrustum(camera.GetProjectionMatrix());
        bounds.Origin = { camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z };
        bounds.Orientation = Quaternion::CreateFromYawPitchRoll(camera.GetDirection().y, camera.GetDirection().x, camera.GetDirection().z);
    }
};

struct QuadTreeBounds
{
    QuadTreeBounds()
    {
        this->xPos = 0;
        this->zPos = 0;
        this->width = 0;
        this->depth = 0;
    }

    QuadTreeBounds(float xPos, float zPos, float width, float depth)
    {
        this->xPos = xPos;
        this->zPos = zPos;
        this->width = width;
        this->depth = depth;
    }

    float xPos;
    float zPos;
    float width;
    float depth;
};

class QuadTree
{

public:
    QuadTree(QuadTreeBounds newBounds, int maxCapacity, int maxlevel, int currentLevel);
    void InsertModel(std::shared_ptr<Drawable>& drawable);
    void GetRelevantDrawables(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, FrustrumCollider frustrumCollider);
    void PrintTree();

private:
    void InsertModelInChild(std::shared_ptr<Drawable>& drawable);
    void DivideQuadTree();
    void DeleteMemory();
    
    //CREATE FROM MATRIX på frustrummet jag vill ha collision check med

    int maxCap;
    std::map<std::string, std::shared_ptr<Drawable>> collectedDrawables;
    //Unorderd Map
    //

    DirectX::BoundingOrientedBox quadTreeBoundsCollider;
    QuadTreeBounds bounds;

    int maxLevel;
    int currentLevel;
    bool divided = false;
    QuadTree* TopR = nullptr;
    QuadTree* TopL = nullptr;
    QuadTree* BotR = nullptr;
    QuadTree* BotL = nullptr;
};