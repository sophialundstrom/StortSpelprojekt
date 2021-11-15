#pragma once
#include "Math.h"
#include "Model.h"
#include "Collision.h"

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
    void InsertModelInChild(std::shared_ptr<Drawable>& drawable);
    void PrintTree();

private:
    void DivideQuadTree();
    void DeleteMemory();
    QuadTreeBounds bounds;
    

    //int cap;
    int maxCap;
    std::map<std::string, std::shared_ptr<Drawable>> collectedDrawables;
    std::shared_ptr<BoundingBox> quadTreeCollider;

    int maxLevel;
    int currentLevel;
    bool divided = false;
    QuadTree* TopR = nullptr;
    QuadTree* TopL = nullptr;
    QuadTree* BotR = nullptr;
    QuadTree* BotL = nullptr;
};