#pragma once
#include "Model.h"
#include "BuildingEffect.h"
#include <functional>
#include <thread>
#include "Collision.h"

class Building :public Model
{
private:
	bool upgrading = false;
	static const UINT stages = 3;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 0;
	float lastUpdate = 0;
	std::shared_ptr<BoundingSphere> collider;
public:
	std::unique_ptr<BuildingEffect> effect;
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &name, Vector3 position, Scene& scene)
		:Model(meshNames[0], name)
	{
		collider = std::make_shared<BoundingSphere>();
		this->position = position;
		collider->SetPosition(position);
		effect = std::make_unique<BuildingEffect>(Vector3{position.x, position.y + 5, position.z}, scene);

		for (UINT i = 0; i < stages; ++i)
		{
			this->meshNames[i] = meshNames[i];
			this->materialNames[i] = materialNames[i];
		}
	}

	std::shared_ptr<BoundingSphere> GetCollider() { return collider; }

	void SetColliderRadius(float radius) 
	{ 
		collider->SetScale(radius); 
		collider->Update(); 
	}

	void MoveCollider(Vector3 pos) { collider->SetPosition(position + pos); }

	void Upgrade()
	{
		if (upgrading)
			return;

		upgrading = true;
		std::thread worker([=] {

			currState++;
			if (currState >= stages)
				return;

			effect->Start();
			Sleep(5000);
			ApplyMesh(meshNames[currState]);
			ApplyMaterial(materialNames[currState]);
			effect->Stop();
			upgrading = false;
		});

		worker.detach();
	}
};