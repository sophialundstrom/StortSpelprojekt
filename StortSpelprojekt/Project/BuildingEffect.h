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
			//BACKSIDE
			Vector3(position.x - 5, position.y + 4, position.z + 17),
			Vector3(position.x - 5, position.y + 9, position.z + 16),
			Vector3(position.x - 5, position.y + 13, position.z + 17),
			Vector3(position.x - 5, position.y + 16, position.z + 16),

			Vector3(position.x, position.y + 4, position.z + 17),
			Vector3(position.x, position.y + 9, position.z + 16),
			Vector3(position.x, position.y + 13, position.z + 17),
			Vector3(position.x, position.y + 16, position.z + 16),
			Vector3(position.x, position.y + 19, position.z + 17),

			Vector3(position.x + 5, position.y + 4, position.z + 17),
			Vector3(position.x + 5, position.y + 9, position.z + 16),
			Vector3(position.x + 5, position.y + 13, position.z + 17),
			Vector3(position.x + 5, position.y + 16, position.z + 16),
			Vector3(position.x + 5, position.y + 19, position.z + 17),
			Vector3(position.x + 5, position.y + 21, position.z + 17),

			Vector3(position.x + 10, position.y + 4, position.z + 17),
			Vector3(position.x + 10, position.y + 9, position.z + 16),
			Vector3(position.x + 10, position.y + 13, position.z + 17),
			Vector3(position.x + 10, position.y + 16, position.z + 16),
			Vector3(position.x + 10, position.y + 19, position.z + 17),

			//FRONTSIDE
			Vector3(position.x - 5, position.y, position.z - 18),
			Vector3(position.x - 5, position.y + 5, position.z - 17),
			Vector3(position.x - 5, position.y + 9, position.z - 18),
			Vector3(position.x - 5, position.y + 11, position.z - 17),
			Vector3(position.x - 5, position.y + 14, position.z - 17),

			Vector3(position.x, position.y, position.z - 18),
			Vector3(position.x, position.y + 4, position.z - 17),
			Vector3(position.x, position.y + 9, position.z - 18),
			Vector3(position.x, position.y + 12, position.z - 17),
			Vector3(position.x, position.y + 14, position.z - 18),
			Vector3(position.x, position.y + 17, position.z - 18),

			Vector3(position.x + 5, position.y , position.z - 18),
			Vector3(position.x + 5, position.y + 4, position.z - 17),
			Vector3(position.x + 5, position.y + 9, position.z - 19),
			Vector3(position.x + 5, position.y + 12, position.z - 17),
			Vector3(position.x + 5, position.y + 14, position.z - 19),
			Vector3(position.x + 5, position.y + 17, position.z - 17),
			Vector3(position.x + 5, position.y + 20, position.z - 17),
			Vector3(position.x + 5, position.y + 22, position.z - 17),

			Vector3(position.x + 10, position.y, position.z - 18),
			Vector3(position.x + 10, position.y + 5, position.z - 17),
			Vector3(position.x + 10, position.y + 9, position.z - 18),
			Vector3(position.x + 10, position.y + 12, position.z - 17),
			Vector3(position.x + 10, position.y + 14, position.z - 18),

			//TOP
			Vector3(position.x + 5, position.y + 24, position.z - 14),
			Vector3(position.x + 5, position.y + 24, position.z - 10),
			Vector3(position.x + 5, position.y + 24, position.z - 6),
			Vector3(position.x + 5, position.y + 24, position.z - 2),
			Vector3(position.x + 5, position.y + 24, position.z + 2),
			Vector3(position.x + 5, position.y + 24, position.z + 6),
			Vector3(position.x + 5, position.y + 24, position.z + 10),

			//TOP-RIGHTSIDE
			Vector3(position.x + 10, position.y + 22, position.z - 14),
			Vector3(position.x + 10, position.y + 19, position.z - 10),
			Vector3(position.x + 10, position.y + 22, position.z - 6),
			Vector3(position.x + 10, position.y + 19, position.z - 2),
			Vector3(position.x + 10, position.y + 22, position.z + 2),
			Vector3(position.x + 10, position.y + 19, position.z + 6),
			Vector3(position.x + 10, position.y + 22, position.z + 10),

			//TOP-LEFTSIDE
			Vector3(position.x, position.y + 22, position.z - 14),
			Vector3(position.x, position.y + 19, position.z - 10),
			Vector3(position.x, position.y + 22, position.z - 6),
			Vector3(position.x, position.y + 19, position.z - 2),
			Vector3(position.x, position.y + 22, position.z + 2),
			Vector3(position.x, position.y + 19, position.z + 6),
			Vector3(position.x, position.y + 22, position.z + 10),

			//LEFT SIDE
			Vector3(position.x - 7, position.y, position.z),
			Vector3(position.x - 8, position.y + 4, position.z),
			Vector3(position.x - 7, position.y + 8, position.z),
			Vector3(position.x - 8, position.y + 12, position.z),

			Vector3(position.x - 7, position.y,		 position.z + 7),
			Vector3(position.x - 8, position.y + 4,  position.z + 5),
			Vector3(position.x - 7, position.y + 8,  position.z + 7),
			Vector3(position.x - 8, position.y + 12, position.z + 5),

			Vector3(position.x - 7, position.y,		 position.z + 12),
			Vector3(position.x - 8, position.y + 4,  position.z + 10),
			Vector3(position.x - 7, position.y + 8,  position.z + 12),
			Vector3(position.x - 8, position.y + 12, position.z + 10),

			Vector3(position.x - 7, position.y,		 position.z - 7),
			Vector3(position.x - 8, position.y + 4,  position.z - 5),
			Vector3(position.x - 7, position.y + 8,  position.z - 7),
			Vector3(position.x - 8, position.y + 12, position.z - 5),
																
			Vector3(position.x - 7, position.y,		 position.z - 12),
			Vector3(position.x - 8, position.y + 4,  position.z - 10),
			Vector3(position.x - 7, position.y + 8,  position.z - 12),
			Vector3(position.x - 8, position.y + 12, position.z - 10),

			//RIGHT SIDE
			Vector3(position.x + 11, position.y, position.z),
			Vector3(position.x + 13, position.y + 4, position.z),
			Vector3(position.x + 11, position.y + 8, position.z),
			Vector3(position.x + 13, position.y + 12, position.z),
							   
			Vector3(position.x + 11, position.y, position.z + 7),
			Vector3(position.x + 13, position.y + 4, position.z + 5),
			Vector3(position.x + 11, position.y + 8, position.z + 7),
			Vector3(position.x + 13, position.y + 12, position.z + 5),
							   
			Vector3(position.x + 11, position.y, position.z + 12),
			Vector3(position.x + 13, position.y + 4, position.z + 10),
			Vector3(position.x + 11, position.y + 8, position.z + 12),
			Vector3(position.x + 13, position.y + 12, position.z + 10),
							   
			Vector3(position.x + 11, position.y, position.z - 7),
			Vector3(position.x + 13, position.y + 4, position.z - 5),
			Vector3(position.x + 11, position.y + 8, position.z - 7),
			Vector3(position.x + 13, position.y + 12, position.z - 5),
							   
			Vector3(position.x + 11, position.y, position.z - 12),
			Vector3(position.x + 13, position.y + 4, position.z - 10),
			Vector3(position.x + 11, position.y + 8, position.z - 12),
			Vector3(position.x + 13, position.y + 12, position.z - 10)
		};

		const std::string systemName = "testSmoke.ps";
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
