#pragma once
#include "ParticleSystem.h"
#include "Renderers.h"
#include "Scene.h"

class BuildingEffect
{
private:
	std::vector<std::shared_ptr<ParticleSystem>> particles;
public:
	BuildingEffect(Vector3 position, Scene& scene)
	{
		const Vector3 positions[] =
		{
			Vector3(position.x, position.y - 5.0f, position.z),
			Vector3(position.x, position.y - 5.0f, position.z)
			
		};

	
		const std::string systemName = "newSmoke.ps";
		for (UINT i = 0; i < ARRAYSIZE(positions); i++)
		{
			auto system = std::make_shared<ParticleSystem>(systemName);
			system->SetPosition(positions[i]);
			particles.emplace_back(system);
			PR->Bind(system);
			scene.AddDrawable("particleSystem_" + systemName + "_" + std::to_string(i), system);
		}

		Stop();
	}

	void Stop()
	{
		for (auto& system : particles)
			system->StopSpawn();
	}

	void Start()
	{
		for (auto& system : particles)
			system->StartSpawn();
	}
};
