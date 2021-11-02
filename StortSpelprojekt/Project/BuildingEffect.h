#pragma once
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

class BuildingEffect
{
public:
	BuildingEffect(Vector3 position)
	{
		const Vector3 positions[] = 
		{ // LEFT SIDE
			Vector3(position.x - 7, position.y, position.z),
			Vector3(position.x - 9, position.y + 6, position.z + 3),
			Vector3(position.x - 7, position.y, position.z + 6),
			Vector3(position.x - 9, position.y + 6, position.z + 9),
			Vector3(position.x - 7, position.y + 8, position.z + 12),
			Vector3(position.x - 9, position.y + 10, position.z + 12),
			Vector3(position.x - 7, position.y, position.z + 12),
			Vector3(position.x - 9, position.y + 10, position.z + 15),		 						
		};

		const std::string systemName = "testSmoke2.ps";
		for (UINT i = 0; i < ARRAYSIZE(positions); i++)
		{
			auto system = std::make_shared<ParticleSystem>(systemName);
			system->SetPosition(positions[i]);
			particles.emplace_back(system);
		}

	}

	void Unbind(Scene& scene, ParticleRenderer& renderer)
	{
	/*	renderer.Unbind(p1);
		scene.DeleteDrawable("testSystem");*/
	}

	void Bind(Scene& scene, ParticleRenderer& renderer)
	{
		int index = 0;
		for (auto system : particles)
		{
			renderer.Bind(system);
			scene.AddDrawable("testSystem" + std::to_string(index), system);
		}
	}

	void Stop()
	{
		for (auto system : particles)
		{
			system->StopSpawn();
		}
	}

	void Start()
	{
		for (auto system : particles)
		{
			system->StartSpawn();
		}
	}

private:
	std::vector<std::shared_ptr<ParticleSystem>> particles;
};
