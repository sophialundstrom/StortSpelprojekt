#pragma once
#include "Collision.h"
#include "Model.h"

typedef enum RESOURCES 
{
	WOOD,
	STONE,
	FOOD,
	NONE
};

class Item :public Model
{
public:
	Item(RESOURCES type)
		:Model("Pyramid"), ID(type)
	{
		
	}

	bool Collision(const Collider *other)
	{
		switch (other->Type())
		{
		case ColliderType::SPHERE:
		    return Collision::Intersection(*dynamic_cast<const BoundingSphere*>(other), bounds);
		case ColliderType::BOX:
			return Collision::Intersection(bounds,*dynamic_cast<const BoundingBox*>(other));
		}
		return false;
	}
	BoundingSphere& GetBounds() { return bounds; }

private:

	BoundingSphere bounds;
	RESOURCES ID;
};

