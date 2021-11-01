#pragma once
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

class BuildingEffect
{
public:
	BuildingEffect(Vector3 position)
	{
		p1 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p1->SetPosition(Vector3(position.x - 4, position.y + 1, position.z - 1));
		p2 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p2->SetPosition(Vector3(position.x - 4, position.y - 1, position.z - 1));
		p3 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p3->SetPosition(Vector3(position.x - 4, position.y, position.z + 1));
		p4 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p4->SetPosition(Vector3(position.x - 4, position.y + 2, position.z - 2));
		/*p5 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p5->SetPosition(Vector3(position.x - 4, position.y + 1, position.z - 1));
		p6 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p6->SetPosition(Vector3(position.x - 4, position.y - 1, position.z - 1));
		p7 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p7->SetPosition(Vector3(position.x - 4, position.y, position.z + 1));
		p8 = std::make_shared<ParticleSystem>("testSmoke.ps");
		p8->SetPosition(Vector3(position.x - 4, position.y + 2, position.z - 2));*/
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
