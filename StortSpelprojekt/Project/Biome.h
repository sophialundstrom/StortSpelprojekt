#pragma once
#include "Collision.h"
#include "BiomeEnum.h"

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

	void Bind(ColliderRenderer& cRenderer)
	{
		for (auto& collider : colliders)
			cRenderer.Bind(collider);
	}

	std::vector<std::shared_ptr<BoundingSphere>> colliders;
	UINT musicSlot;
	BIOME type;

};