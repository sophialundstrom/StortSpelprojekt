#include "Item.h"

Item::Item(Type type, const std::string& name)
	:type(type)
{
	SetName(name);

	collider = std::make_shared<BoundingSphere>();

	//MAYB SWITCH HERE WITH 

	ApplyMesh(Names[UINT(type)]);
	ApplyMaterial(Names[UINT(type)]);
}

void Item::Update()
{
	Model::Update();
	collider->Update();
}