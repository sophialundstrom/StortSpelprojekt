#pragma once
#include "Collision.h"
#include "BiomeEnum.h"

struct Biome
{
	Biome(const std::wstring& wavPath, BIOME type)
	{/*
		collider = std::make_shared<BoundingSphere>();
		collider->SetScale(radius);
		collider->SetPosition(position);*/
		this->musicName = wavPath;
		this->type = type;
		//collider->Update();
	}

	void AddCollider(const Vector3& position, const float& radius)
	{
		auto collider = std::make_shared<BoundingSphere>();
		collider->SetScale(radius);
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
	std::wstring musicName;
	BIOME type;

};