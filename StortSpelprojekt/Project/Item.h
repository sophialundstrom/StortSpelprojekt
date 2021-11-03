#pragma once
#include "Collision.h"
#include "Model.h"

typedef enum RESOURCE 
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
		:Model("Stick", name), ID(type)
	{
		bounds = std::make_shared<BoundingSphere>();
		//bounds->SetScale(2, 0.5f, 0.5f);
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