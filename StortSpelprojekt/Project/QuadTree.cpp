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
	
	DirectX::BoundingOrientedBox drawableBounds;
	drawableBounds.CreateFromPoints(drawableBounds, 2, drawableAsModel->GetMeshBoundingBoxValues(), sizeof(Vector3));
	
	
	/*Quaternion quat = drawableBounds.Orientation;
	quat.Normalize();
	drawableBounds.Orientation = quat;*/
	
	drawableBounds.Transform(drawableBounds, drawableAsModel->GetMatrix());
	
	/*quat = drawableBounds.Orientation;
	quat.Normalize();
	drawableBounds.Orientation = quat;*/

	bool insideLeaf = drawableBounds.Intersects(quadTreeBoundsCollider);

	if (insideLeaf)
	{
		if (!divided)
		{
			if (collectedDrawables.size() >= maxCap && currentLevel < maxLevel)
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
	if (divided)
	{
		TopL->PrintTree();
		TopR->PrintTree();
		BotL->PrintTree();
		BotR->PrintTree();
	}
	else
	{
		std::cout << "---------------------------------------------------------\n";
		std::cout << "QuadTree LeafBounds:\n";
		std::cout << "xPosMin: " << bounds.xPos << std::endl;
		std::cout << "zPosMin: " << bounds.zPos << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "xPosMax: " << bounds.xPos + bounds.width << std::endl;
		std::cout << "zPosMax: " << bounds.zPos + bounds.depth << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "xWidth: " << bounds.width << std::endl;
		std::cout << "zDepth: " << bounds.depth << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "LayerDepth: " << currentLevel << std::endl;
		std::cout << "MaxDepth: " << maxLevel << std::endl;
		std::cout << "ActiveEntities: " << collectedDrawables.size() << std::endl;
		std::cout << "MaxEntityCount: " << maxCap << std::endl;
	}

}

void QuadTree::DivideQuadTree()
{
	TopL = new QuadTree(QuadTreeBounds(bounds.xPos, bounds.zPos, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	TopR = new QuadTree(QuadTreeBounds(bounds.xPos + bounds.width / 2.f, bounds.zPos, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	BotL = new QuadTree(QuadTreeBounds(bounds.xPos, bounds.zPos + bounds.depth / 2.f, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);
	BotR = new QuadTree(QuadTreeBounds(bounds.xPos + bounds.width / 2.f, bounds.zPos + bounds.depth / 2.f, bounds.width / 2.f, bounds.depth / 2.f), maxCap, maxLevel, currentLevel + 1);

	for (auto& [name, drawable] : collectedDrawables)
		InsertModelInChild(drawable);

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
