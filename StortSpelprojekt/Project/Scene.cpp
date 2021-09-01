#include "Scene.h"
#include "ShaderData.h"
#include "SettingsLoader.h"
#include "Event.h"
#include <filesystem>

void Scene::LoadModels()
{
	for (const auto& directory : std::filesystem::directory_iterator("Models"))
	{
		std::string fileName = directory.path().filename().string();
		SettingsLoader::ApplySettings(std::make_shared<Model>(fileName), fileName, drawables);
	}
}

void Scene::AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type)
{
	auto particleSystem = std::make_shared<ParticleSystem>(maxParticles, timeBetweenParticles, particlesLifetime, minVelocity, maxVelocity, size, particleExtents, position, type);
	particleSystem->BindToRenderGraph();
	drawables.emplace("Particle System " + std::to_string(numParticleSystems), particleSystem);
	numParticleSystems++;
}

void Scene::AddPointLight(Vector3 position, float range, Vector3 attenuation, Vector4 color)
{
	if (pointLights.size() < MAX_LIGHTS)
		pointLights.push_back(PointLight(position, range, attenuation, color));
}

void Scene::Initialize(UINT width, UINT height)
{
	player = Player(PI_DIV4, (float)width / (float)height, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 5.0f, -10.0f });
	directionalLight = DirectionalLight(30);

	LoadModels();

	// RAIN PARTICLE SYSTEM
	AddParticleSystem(500, 0.1f, 2.0f, 12.0f, 20.0f, 40.0f, { 0.05f, 0.08f }, { 0.0f, 30.0f, 0.0f }, EmitterType::CUBE);

	//HOUSE LIGHT
	AddPointLight({ -10.0f, 8.0f, 1.0f }, 40.0f);
}

void Scene::ShutDown()
{
	drawables.clear();
	Resources::ShutDown();
}

void Scene::Update()
{
	player.Update();
	directionalLight.Update();

	for (auto& [name, drawable] : drawables)
		drawable->Update();

	ShaderData::Update(player.GetCamera(), directionalLight, (UINT)pointLights.size(), pointLights.data());
}