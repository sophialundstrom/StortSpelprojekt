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
        //bounds.Origin = { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z };
        //bounds.Orientation = Quaternion::CreateFromYawPitchRoll(camera->GetDirection().x, camera->GetDirection().y, camera->GetDirection().z);
        bounds = DirectX::BoundingFrustum();
        bounds.CreateFromMatrix(bounds, camera->GetProjectionMatrix());
        bounds.Transform(
            bounds, 
            1, 
            Quaternion::CreateFromYawPitchRoll(camera->GetDirection().x, camera->GetDirection().y, camera->GetDirection().z), 
            { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z }
        );

        Quaternion q = Quaternion::CreateFromYawPitchRoll(camera->GetDirection().x, camera->GetDirection().y, camera->GetDirection().z);
        //std::cout << "--\n";
        //std::cout << q.x << "           " << q.y << "           " << q.z << "           " << q.w << std::endl;
        //std::cout << bounds.Orientation.x << "           " << bounds.Orientation.y << "           " << bounds.Orientation.z << "           " << bounds.Orientation.w << std::endl;
        //std::cout << "--\n";
        //std::cout << bounds.Far << std::endl;
        
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
    void GetRelevantDrawables(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, FrustrumCollider frustrumCollider);
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