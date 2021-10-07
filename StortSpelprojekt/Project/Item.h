#pragma once
#include "Collision.h"
#include "Player.h"
#include "Model.h"

typedef enum Items 
{
	WOOD,
	STONE,
	FOOD,
	NONE
};

class Item :public Model
{
public:
	bool Collision(const Collider *other)
	{
		switch (other->Type())
		{
		default:
			return false;
		case ColliderType::SPHERE:
			return Collision::Intersection(*dynamic_cast<const BoundingSphere*>(other), bounds);
		case ColliderType::BOX:
			return Collision::Intersection(bounds,*dynamic_cast<const BoundingBox*>(other));
		}
	}

private:

	BoundingSphere bounds;
	Items ID;
};

