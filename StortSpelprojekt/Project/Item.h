#pragma once
#include "Collision.h"
#include "Model.h"

enum class RESOURCE
{
	WOOD,
	STONE,
	FOOD,
	NONE
};

class Item :public Model
{
public:
	static const std::string Names[];

	Item(RESOURCE type, const std::string& name)
		:Model("Pyramid", name), ID(type)
	{
		bounds = std::make_shared<BoundingSphere>();
	}

	bool Collision(const Collider *other)
	{
		switch (other->Type())
		{
		case ColliderType::SPHERE:
		    return Collision::Intersection(*dynamic_cast<const BoundingSphere*>(other), *bounds);
		case ColliderType::BOX:
			return Collision::Intersection(*bounds,*dynamic_cast<const BoundingBox*>(other));
		}
		return false;
	}

	void Update()
	{
		Model::Update();
		bounds->Update();
	}

	RESOURCE GetType() { return ID; }

	std::shared_ptr<BoundingSphere> GetBounds() { return bounds; }

private:

	std::shared_ptr<BoundingSphere> bounds;
	RESOURCE ID;
};

inline const std::string Item::Names[] = { "WOOD", "STONE", "FOOD" };