#pragma once
#include "Model.h"
#include "BuildingEffect.h"
#include <functional>
#include <thread>
#include "Collision.h"

class Building :public Model
{
private:
	static const UINT stages = 3;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 1;
	float lastUpdate = 0;
	std::shared_ptr<BoundingSphere> collider;
	std::string buildingName = "";
public:
	int reqStick1 = 0;
	int reqStone1 = 0;
	int reqStick2 = 0;
	int reqStone2 = 0;
	bool upgrading = false;
	std::unique_ptr<BuildingEffect> effect;
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &name, Vector3 position, Scene& scene, const std::string &systemName)
		:Model(meshNames[0], name)
	{
		buildingName = name;
		collider = std::make_shared<BoundingSphere>();
		this->position = position;
		collider->SetPosition(position);
		effect = std::make_unique<BuildingEffect>(Vector3{position.x, position.y + 5, position.z}, scene, systemName);

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
	void SetRequirements(int stick1, int stone1, int stick2, int stone2) { reqStick1 = stick1; reqStick2 = stick2; reqStone1 = stone1; reqStone2 = stone2; }


	void MoveCollider(Vector3 pos) { collider->SetPosition(position + pos); }
	int GetCurrentState() { return currState; }
	std::string GetBuildingName() { return buildingName; }

	void Upgrade()
	{
		if (upgrading)
			return;

		upgrading = true;
		std::thread worker([=] {

			if (currState >= stages)
				return;

			effect->Start();
			Sleep(5000);
			ApplyMesh(meshNames[currState]);
			ApplyMaterial(materialNames[currState]);
			currState++;
			effect->Stop();
			upgrading = false;
		});

		worker.detach();
	}
};