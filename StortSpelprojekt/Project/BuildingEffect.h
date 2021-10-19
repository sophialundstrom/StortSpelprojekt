#pragma once
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

class BuildingEffect
{
public:
	
	BuildingEffect(Vector3 position)
	{
		p1 = std::make_shared<ParticleSystem>("rain.ps");
		p1->SetPosition(position);
	}
	void Unbind(Scene& scene, ParticleRenderer& renderer)
	{
		renderer.Unbind(p1);
		scene.DeleteDrawable("testSystem");
	}
	void Bind(Scene& scene, ParticleRenderer& renderer)
	{
		renderer.Bind(p1);
		scene.AddParticleSystem("testSystem", p1);
	}
	void Stop()
	{
		p1->StopSpawn();
	}
	void Start()
	{
		p1->StartSpawn();
	}
private:
	std::shared_ptr<ParticleSystem> p1;
};
