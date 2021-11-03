#pragma once
#include "Model.h"
#include "BuildingEffect.h"
#include <functional>
#include <thread>

class Building :public Model
{
private:
	bool upgrading = false;
	static const UINT stages = 3;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 0;
	float lastUpdate = 0;
	std::function<void(Scene&, ParticleRenderer&, std::unique_ptr<BuildingEffect> &)> unbindCallback;
public:
	std::unique_ptr<BuildingEffect> effect;
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &name, Vector3 position, Scene& scene, ParticleRenderer& renderer)
		:Model(meshNames[0], name)
	{
		unbindCallback = [this](Scene& scene, ParticleRenderer& renderer, std::unique_ptr<BuildingEffect> &effect) {Unbind(scene, renderer, effect); };
		this->position = position;
		effect = std::make_unique<BuildingEffect>(Vector3{position.x, position.y + 5, position.z});
		effect->Bind(scene, renderer);
		effect->Stop();

		for (UINT i = 0; i < stages; ++i)
		{
			this->meshNames[i] = meshNames[i];
			this->materialNames[i] = materialNames[i];
		}
	}

	void Upgrade()
	{
		if (upgrading)
			return;

		upgrading = true;
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
			upgrading = false;
			//done.store(false);

		});

		worker.detach();

	}

	void Unbind(Scene& scene, ParticleRenderer& renderer, std::unique_ptr<BuildingEffect> &effect)
	{
		int index = 0;
		for (auto system : effect->particles)
		{
			renderer.Unbind(system);
			scene.DeleteDrawable("testSystem" + std::to_string(index));
		}
	}
};