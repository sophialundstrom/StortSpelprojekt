#pragma once
#include "Collision.h"
#include "Model.h"

class Item : public Model
{
public:
	enum class Type { Stick, Stone, Food, Hammer, Axe }; // FILL THIS WITH ALL ITEMS & IN NAMES ARRAY (BOTTOM OF FILE)
	static const std::string Names[];
private:
	std::shared_ptr<BoundingSphere> collider;
	Type type;
public:
	Item(Type type, const std::string& name);

	void Update();

	std::shared_ptr<BoundingSphere> GetCollider()	{ return collider; }
	Type GetType()									{ return type; }
};

inline const std::string Item::Names[] = { "Stick", "Tomato", "Food", "Hammer", "Axe" };

//class Item :public Model
//{
//public:
//	static const std::string Names[];
//
//	Item(RESOURCE type, const std::string& name)
//		:Model("Stick", name), ID(type)
//	{
//		bounds = std::make_shared<BoundingSphere>();
//		//bounds->SetScale(2, 0.5f, 0.5f);
//	}
//
//	void Update()
//	{
//		Model::Update();
//		bounds->Update();
//	}
//
//	RESOURCE GetType() { return ID; }
//
//	std::shared_ptr<BoundingSphere> GetBounds() { return bounds; }
//public:
//	enum class Type { Wood, Stone, Food, Axe };
//private:
//
//	std::shared_ptr<BoundingSphere> bounds;
//	RESOURCE ID;
//};
//
//inline const std::string Item::Names[] = { "WOOD", "STONE", "FOOD" };