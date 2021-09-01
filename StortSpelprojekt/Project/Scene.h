#pragma once
#include "Light.h"
#include "Model.h"
#include "ParticleSystem.h"
#include "Player.h"

#include <map>

class Scene
{
private:
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;

	Player player;

	UINT numParticleSystems = 0;

	std::map<std::string, std::shared_ptr<Drawable>> drawables;

	void LoadModels();
	void AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	void AddPointLight(Vector3 position, float range, Vector3 attenuation = { 0.05f, 0.05f, 0.05f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
public:
	Scene() = default;
	void Initialize(UINT width, UINT height);
	void ShutDown();
	void Update();
};