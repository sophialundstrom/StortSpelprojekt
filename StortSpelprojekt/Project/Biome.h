#pragma once
#include "Collision.h"
#include "BiomeEnum.h"
#include "Renderers.h"

struct Biome
{
	Biome(const UINT& slot, BIOME type)
	{
		this->musicSlot = slot;
		this->type = type;
	}

	void AddCollider(const Vector3& position, const float& scale)
	{
		auto collider = std::make_shared<BoundingSphere>();
		collider->SetScale(scale);
		collider->SetPosition(position); 
		collider->Update();
		colliders.emplace_back(collider);
	}

	void Bind()
	{
		for (auto& collider : colliders)
			CR->Bind(collider);
	}

	std::vector<std::shared_ptr<BoundingSphere>> colliders;
	UINT musicSlot;
	BIOME type;

};