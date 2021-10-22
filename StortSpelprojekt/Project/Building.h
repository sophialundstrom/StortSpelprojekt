#pragma once
#include "Model.h"
#include "BuildingEffect.h"
#include <functional>
#include <thread>

class Building :public Model
{
private:
	static const UINT stages = 2;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 0;
	float lastUpdate = 0;
public:
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &name, Vector3 position)
		:Model(meshNames[0], name)
	{
		this->position = position;
		effect = std::make_unique<BuildingEffect>(Vector3{position.x, position.y + 5, position.z});
		for (UINT i = 0; i < stages; ++i)
		{
			this->meshNames[i] = meshNames[i];
			this->materialNames[i] = materialNames[i];
		}
	}

	void Upgrade()
	{
		std::thread worker([=] {

			currState++;
			if (currState >= stages)
				return;
			//done.store(true);
			effect->Start();
			Sleep(5000);
			ApplyMesh(meshNames[currState]);
			ApplyMaterial(materialNames[currState]);
			effect->Stop();
			//done.store(false);

		});

		worker.detach();

	}
	std::unique_ptr<BuildingEffect> effect;

	/*void Upgrade()
	{
		if (Time::Get() - lastUpdate < 1.0f)
			return;

		lastUpdate = Time::Get();

		currState++;
		if (currState >= stages)
			return;

		ApplyMesh(meshNames[currState]);
		ApplyMaterial(materialNames[currState]);
	}*/
};