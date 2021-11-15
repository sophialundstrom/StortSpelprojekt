#include "QuadTree.h"



QuadTree::QuadTree(QuadTreeBounds newBounds, int maxCapacity, int maxlevel, int currentLevel)
{
	this->currentLevel = currentLevel;
	this->maxLevel = maxlevel;
	maxCap = maxCapacity;
	bounds = newBounds;
	quadTreeCollider = std::make_shared<BoundingBox>();
	quadTreeCollider->SetPosition({ bounds.xPos + (bounds.width/2.f), 0, bounds.zPos + (bounds.depth/2.f) });
	quadTreeCollider->SetScale(bounds.width, 1000, bounds.depth);
}

void QuadTree::InsertModel(std::shared_ptr<Drawable>& drawable)
{
	//bool addCollider = Collision::Intersection(quadTreeCollider, );

	auto drawableAsModel = std::dynamic_pointer_cast<Model>(drawable);
	Vector3 minVals, maxVals;
	drawableAsModel->GetMeshBoundingBoxValues(minVals, maxVals);
	Vector3 minMaxVals[2] = { minVals, maxVals };
	DirectX::BoundingOrientedBox bounds;
	bounds.CreateFromPoints(bounds, 2, minMaxVals, sizeof(Vector3));
	bounds.Transform(bounds, drawableAsModel->GetMatrix());


	if (!divided)
	{
		if (collectedDrawables.size() >= maxCap)
		{
			DivideQuadTree();
			InsertModelInChild(drawable);
		}
		else
			collectedDrawables.emplace(drawable->GetName(), drawable);
	}
	else
		InsertModelInChild(drawable);
}

void QuadTree::InsertModelInChild(std::shared_ptr<Drawable>& drawable)
{
}

void QuadTree::PrintTree()
{

}

void QuadTree::DivideQuadTree()
{
	TopL = new QuadTree(QuadTreeBounds(bounds.xPos, bounds.zPos, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	TopR = new QuadTree(QuadTreeBounds(bounds.xPos + bounds.width / 2.f, bounds.zPos, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	BotL = new QuadTree(QuadTreeBounds(bounds.xPos, bounds.zPos + bounds.depth / 2.f, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	BotR = new QuadTree(QuadTreeBounds(bounds.xPos + bounds.width / 2.f, bounds.zPos + bounds.depth / 2.f, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);

	divided = true;
}

void QuadTree::DeleteMemory()
{
	if (divided)
	{
		delete TopL;
		delete TopR;
		delete BotL;
		delete BotR;
	}
	
}
