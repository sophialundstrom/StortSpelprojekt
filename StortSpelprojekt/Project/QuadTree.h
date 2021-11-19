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
        //https://stackoverflow.com/questions/58469297/how-do-i-calculate-the-yaw-pitch-and-roll-of-a-point-in-3d
        //Yaw/Heading Pitch/Pitch Roll/Bank
        //float heading, pitch, bank;

        ////Calculate Heading
        //DirectX::XMFLOAT3 camDir = camera->GetDirection();
        //float magnitude = (sqrtf(pow(camDir.x, 2) + pow(camDir.y, 2) + pow(camDir.z, 2)));
        //float redAdjacent = camDir.x;
        //float redOpposite = camDir.z;
        //heading = atan2f(redOpposite, redAdjacent);

        ////Calculate Pitch
        //DirectX::XMFLOAT3 origin = { 0, 0, 0 };
        //DirectX::XMFLOAT3 xzPlane = { redAdjacent, 0, redOpposite };

        //float greenHypotenuse = magnitude;
        //float greenOpposite = camDir.y;



        Quaternion camDirQ = Quaternion::CreateFromYawPitchRoll(camera->GetJaw(), camera->GetPitch(), 0.f);
        

        bounds = DirectX::BoundingFrustum();
        bounds.CreateFromMatrix(bounds, camera->GetProjectionMatrix());
        bounds.Transform(
            bounds,
            1,
            camDirQ,
            { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z }
        );
        
        

        bounds.Far = camera->GetFarZ();
        bounds.Near = camera->GetNearZ();
        
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