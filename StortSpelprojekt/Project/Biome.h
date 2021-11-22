#pragma once
#include "Collision.h"
#include "BiomeEnum.h"

struct Biome
{
	Biome(const Vector3& position, const float& radius, const std::string& wavPath, BIOME type)
	{
		collider = std::make_shared<BoundingSphere>();
		collider->SetScale(radius);
		collider->SetPosition(position);
		this->musicName = wavPath;
		this->type = type;
		collider->Update();
	}

	std::shared_ptr<BoundingSphere> collider;
	std::string musicName;
	BIOME type;

};