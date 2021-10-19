#pragma once
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

class BuildingEffect
{
public:
	
	BuildingEffect(Vector3 position)
	{
		p1 = std::make_shared<ParticleSystem>("smoke.ps");
		p1->SetPosition(position);
		p2 = std::make_shared<ParticleSystem>("smoke1.ps");
		p2->SetPosition(position);
		p3 = std::make_shared<ParticleSystem>("smoke2.ps");
		p3->SetPosition(position);
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
		renderer.Bind(p2);
		scene.AddParticleSystem("testSystem", p2);
		renderer.Bind(p3);
		scene.AddParticleSystem("testSystem", p3);
	}
	void Stop()
	{
		p1->StopSpawn();
		p2->StopSpawn();
		p3->StopSpawn();

	}
	void Start()
	{
		p1->StartSpawn();
		p2->StartSpawn();
		p3->StartSpawn();

	}
private:
	std::shared_ptr<ParticleSystem> p1;
	std::shared_ptr<ParticleSystem> p2;
	std::shared_ptr<ParticleSystem> p3;
};
