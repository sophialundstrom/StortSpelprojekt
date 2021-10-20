#pragma once
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

class BuildingEffect
{
public:
	BuildingEffect(Vector3 position)
	{
		p1 = std::make_shared<ParticleSystem>("smoke3.ps");
		p1->SetPosition(position);
		p2 = std::make_shared<ParticleSystem>("smoke3.ps");
		p2->SetPosition(position);
		p3 = std::make_shared<ParticleSystem>("smoke3.ps");
		p3->SetPosition(position);
		p4 = std::make_shared<ParticleSystem>("smoke3.ps");
		p4->SetPosition(position);
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
		renderer.Bind(p4);
		scene.AddParticleSystem("testSystem", p4);
	}
	void Stop()
	{
		p1->StopSpawn();
		p2->StopSpawn();
		p3->StopSpawn();
		p4->StopSpawn();
	}
	void Start()
	{
		p1->StartSpawn();
		p2->StartSpawn();
		p3->StartSpawn();
		p4->StartSpawn();
	}
private:
	std::shared_ptr<ParticleSystem> p1;
	std::shared_ptr<ParticleSystem> p2;
	std::shared_ptr<ParticleSystem> p3;
	std::shared_ptr<ParticleSystem> p4;
};
