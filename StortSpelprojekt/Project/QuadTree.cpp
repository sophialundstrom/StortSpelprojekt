#include "QuadTree.h"

QuadTree::QuadTree(QuadTreeBounds newBounds, int maxCapacity, int maxlevel, int currentLevel, std::string nameTag)
{
	this->currentLevel = currentLevel;
	this->maxLevel = maxlevel;
	maxCap = maxCapacity;
	bounds = newBounds;
	this->nameTag = nameTag;

	quadTreeBoundsCollider = DirectX::BoundingBox(
		{ bounds.xPos + (bounds.width / 2.f), 0, bounds.zPos + (bounds.depth / 2.f) },
		{ bounds.width / 2.f, 4000.f, bounds.depth / 2.f }
	);

	lowestY = D3D11_FLOAT32_MAX;
	highestY = -D3D11_FLOAT32_MAX;


}

void QuadTree::InsertModel(std::shared_ptr<Drawable>& drawable)
{
	auto drawableAsModel = std::dynamic_pointer_cast<Model>(drawable);
	
	DirectX::BoundingOrientedBox drawableBounds;
	drawableBounds.CreateFromPoints(drawableBounds, 2, drawableAsModel->GetMeshBoundingBoxValues(), sizeof(Vector3));
	
	drawableBounds.Transform(drawableBounds, drawableAsModel->GetMatrix());
	
	Vector3 boundTransform{
		drawableBounds.Center.x + drawable->GetPosition().x, 
		drawableBounds.Center.y + drawable->GetPosition().y,
		drawableBounds.Center.z + drawable->GetPosition().z
	};
	drawableBounds.Center = boundTransform;
	
	drawableBounds.Extents.x *= drawable->GetScale().x;
	drawableBounds.Extents.y *= drawable->GetScale().y;
	drawableBounds.Extents.z *= drawable->GetScale().z;

	//IF Stuff is still broken try tweak with orientation but why risk it if stuff seems to be working?
	drawableBounds.Orientation = drawable->GetRotation();

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
			{
				float sizes[] = {
					drawableBounds.Extents.x,
					drawableBounds.Extents.y,
					drawableBounds.Extents.z
				};
				for (int i = 0; i < 3; i++)
				{
					if (lowestY > drawableBounds.Center.y - sizes[i])
						lowestY = (drawableBounds.Center.y - sizes[i]);

					if (highestY < drawableBounds.Center.y + sizes[i])
						highestY = (drawableBounds.Center.y + sizes[i]);
				}

				collectedDrawables.emplace(drawable->GetName(), drawable);
			}
		}
		else
			InsertModelInChild(drawable);
	}
}

void QuadTree::CheckModelsWithinView(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, FrustrumCollider frustrumCollider)
{
	if (divided)
	{
		TopL->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);
		TopR->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);
		BotL->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);
		BotR->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);
	}
	else
	{


		if (quadTreeBoundsCollider.Intersects(frustrumCollider.bounds))
		{
			//std::cout << "Intersects " + nameTag << std::endl;
			for (auto& [name, drawable] : collectedDrawables)
				drawablesToBeRendered.emplace(name, drawable);
		}
	}
}

void QuadTree::CheckModelsWithinView(std::map<std::string, std::shared_ptr<Drawable>>& drawablesToBeRendered, OrthographicCollider orthographicCollider)
{
	if (divided)
	{
		TopL->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);
		TopR->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);
		BotL->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);
		BotR->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);
	}
	else
	{


		if (quadTreeBoundsCollider.Intersects(orthographicCollider.bounds))
		{
			//std::cout << "Intersects " + nameTag << std::endl;
			for (auto& [name, drawable] : collectedDrawables)
				drawablesToBeRendered.emplace(name, drawable);
		}
	}
}

void QuadTree::GetAllDrawables(std::map<std::string, std::shared_ptr<Drawable>>& allDrawables)
{
	if (divided)
	{
		TopL->GetAllDrawables(allDrawables);
		TopR->GetAllDrawables(allDrawables);
		BotL->GetAllDrawables(allDrawables);
		BotR->GetAllDrawables(allDrawables);
	}
	else
	{
		for (auto& [name, drawable] : collectedDrawables)
			allDrawables.emplace(name, drawable);
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

void QuadTree::OptimizeBounds()
{
	if (divided)
	{
		TopL->OptimizeBounds();
		TopR->OptimizeBounds();
		BotL->OptimizeBounds();
		BotR->OptimizeBounds();
	}
	else
	{
		if (lowestY == D3D11_FLOAT32_MAX)
			lowestY = -0.5f;

		if (highestY == -D3D11_FLOAT32_MAX)
			highestY = 0.5f;

		if (lowestY > highestY)
		{
			float temp = highestY;
			highestY = lowestY;
			lowestY = temp;
		}

		/*if (highestY < lowestY + 400)
			highestY += 400;*/

		/*lowestY -= 100;
		highestY += 100;*/

		std::cout << "---\n";
		std::cout << "lowestY: " << lowestY << std::endl;
		std::cout << "HighestY: " << highestY << std::endl;

		

		

		Vector3 newExtent = { quadTreeBoundsCollider.Extents.x, (highestY - lowestY) / 2.f, quadTreeBoundsCollider.Extents.z };
		Vector3 newYcenter = { quadTreeBoundsCollider.Center.x, lowestY + newExtent.y, quadTreeBoundsCollider.Center.z };
		quadTreeBoundsCollider.Center = newYcenter;
		quadTreeBoundsCollider.Extents = newExtent;
	}
}

void QuadTree::DivideQuadTree()
{
	float newWidth = bounds.width / 2.f;
	float newDepth = bounds.depth / 2.f;
	QuadTreeBounds TopLB(bounds.xPos, bounds.zPos, newWidth, newDepth);
	QuadTreeBounds TopRB(bounds.xPos + newWidth, bounds.zPos, newWidth, newDepth);
	QuadTreeBounds BotLB(bounds.xPos, bounds.zPos + newDepth, newWidth, newDepth);
	QuadTreeBounds BotRB(bounds.xPos + newWidth, bounds.zPos + newDepth, newWidth, newDepth);

	TopL = new QuadTree(TopLB, maxCap, maxLevel, currentLevel + 1, "TOPLEFT");
	TopR = new QuadTree(TopRB, maxCap, maxLevel, currentLevel + 1, "TOPRIGHT");
	BotL = new QuadTree(BotLB, maxCap, maxLevel, currentLevel + 1, "BOTLEFT");
	BotR = new QuadTree(BotRB, maxCap, maxLevel, currentLevel + 1, "BOTRIGHT");

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
