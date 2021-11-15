#include "QuadTree.h"

QuadTree::QuadTree(QuadTreeBounds newBounds, int maxCapacity, int maxlevel, int currentLevel)
{
	this->currentLevel = currentLevel;
	this->maxLevel = maxlevel;
	maxCap = maxCapacity;
	bounds = newBounds;
	Vector3 minMaxVals[2] = { {bounds.xPos, -500, bounds.zPos}, { bounds.xPos + bounds.width, 500, bounds.zPos + bounds.depth } };
	quadTreeBoundsCollider.CreateFromPoints(quadTreeBoundsCollider, 2, minMaxVals, sizeof(Vector3));
}

void QuadTree::InsertModel(std::shared_ptr<Drawable>& drawable)
{
	auto drawableAsModel = std::dynamic_pointer_cast<Model>(drawable);
	Vector3 minVals, maxVals;
	drawableAsModel->GetMeshBoundingBoxValues(minVals, maxVals);
	Vector3 minMaxVals[2] = { minVals, maxVals };
	DirectX::BoundingOrientedBox drawableBounds;
	drawableBounds.CreateFromPoints(drawableBounds, 2, minMaxVals, sizeof(Vector3));
	drawableBounds.Transform(drawableBounds, drawableAsModel->GetMatrix());

	bool insideLeaf = drawableBounds.Intersects(quadTreeBoundsCollider);

	if (insideLeaf)
	{
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
}

void QuadTree::InsertModelInChild(std::shared_ptr<Drawable>& drawable)
{
	TopL->InsertModel(drawable);
	TopR->InsertModel(drawable);
	BotL->InsertModel(drawable);
	BotR->InsertModel(drawable);
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
