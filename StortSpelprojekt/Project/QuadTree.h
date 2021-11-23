#pragma once
#include "Math.h"
#include "Model.h"
#include "Camera.h"

struct FrustrumCollider
{
    DirectX::BoundingFrustum bounds;
    DirectX::XMMATRIX viewMatrixData; 
    void SetupFrustrum(Camera camera)
    {
        bounds = DirectX::BoundingFrustum();
        bounds.CreateFromMatrix(bounds, camera.GetProjectionMatrix());
    }

    void Update(Camera* camera)
    {
        Quaternion camDirQ = Quaternion::CreateFromYawPitchRoll(camera->GetJaw(), camera->GetPitch(), 0.f);
        
        bounds = DirectX::BoundingFrustum();
        bounds.CreateFromMatrix(bounds, camera->GetProjectionMatrix());
       
        bounds.Transform(
            bounds,
            1,
            camDirQ,
            { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z }
        );
        
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
    QuadTree(QuadTreeBounds newBounds, int maxCapacity, int maxlevel, int currentLevel, std::string nameTag);
    void InsertModel(std::shared_ptr<Drawable>& drawable);
    void CheckModelsWithinFustrum(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, FrustrumCollider frustrumCollider);
    void GetAllDrawables(std::map<std::string, std::shared_ptr<Drawable>>& allDrawables);
    void PrintTree();

private:
    void InsertModelInChild(std::shared_ptr<Drawable>& drawable);
    void DivideQuadTree();
    void DeleteMemory();
    
    std::string nameTag;
    //CREATE FROM MATRIX på frustrummet jag vill ha collision check med

    int maxCap;
    std::map<std::string, std::shared_ptr<Drawable>> collectedDrawables;
    //Unorderd Map
    //

    DirectX::BoundingBox quadTreeBoundsCollider;
    QuadTreeBounds bounds;

    int maxLevel;
    int currentLevel;
    bool divided = false;
    QuadTree* TopR = nullptr;
    QuadTree* TopL = nullptr;
    QuadTree* BotR = nullptr;
    QuadTree* BotL = nullptr;
};