#pragma once
#include "Math.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

struct FrustrumCollider
{
    DirectX::BoundingFrustum bounds;
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

struct OrthographicCollider
{
    DirectX::BoundingOrientedBox bounds;
    void Update(DirectionalLight dirLight)
    {
        
        // https://stackoverflow.com/questions/58469297/how-do-i-calculate-the-yaw-pitch-and-roll-of-a-point-in-3d
        /*
        //YAW                          Heading
        //PITCH                        Pitch
        //ROLL                         Bank

        //Calculating magnitude
        Vector3 dir = dirLight.data.direction;
        float magnitude = sqrtf((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

        //Calculate heading
        float redAdjacent = dir.x;
        float redOpposite = dir.z;
        float heading = atan2f(redOpposite, redAdjacent);

        //Calculating pitch
        //float greenHypotenuse = magnitude;
        float greenOpposite = dir.y;
        float greenAdjacent = sqrtf((dir.x * dir.x) + (dir.z * dir.z));
        float pitch = atan2(greenOpposite, greenAdjacent);
        */
        
        float downSacle = 0.01;
        float lightRange = dirLight.GetRange();
        Vector3 direction = dirLight.data.direction;
        Vector3 lightPos = dirLight.GetRepresentativePosition();
        float pitch = asinf(direction.y);
        float yaw = atan2f(direction.x, direction.z);

        Quaternion camDirQ = Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0);
        bounds.Extents = { lightRange * downSacle, lightRange * downSacle, lightRange * 2 };
        bounds.Orientation = camDirQ;
        bounds.Center = lightPos + (direction * (-lightRange * 0.5f));
        
        
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
    void CheckModelsWithinView(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, FrustrumCollider frustrumCollider);
    void CheckModelsWithinView(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, OrthographicCollider orthographicCollider);
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