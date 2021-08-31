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

		if (fileName == "Pedestal" || fileName == "Diamond")
		{
			if (pedestals.size() == 0)
			{
				const UINT numSets = 4;
				Vector3 diamondPositions[] = { {15.0f, 0.5f, -10.0f}, {-6.0f, 3.5f, 3.0f}, {-19.0f, 1.5f, -15.0f}, {8.0f, 0.5f, 8.0f} };
				Vector3 pedestalPositions[] = { {22.0f, 3.5f, -2.0f}, {-4.0f, 3.5f, 20.0f}, {-15.0f, 3.5f, -12.0f}, {-4.0f, 3.5f, -22.0f} };
				for (UINT i = 0; i < numSets; ++i)
				{
					auto diamond = std::make_shared<Diamond>(diamondPositions[i]);
					diamond->BindToRenderGraph();
					diamonds.push_back(diamond);
					drawables.emplace("Diamond" + std::to_string(i), diamond);

					auto pedestal = std::make_shared<Pedestal>(pedestalPositions[i]);
					pedestal->BindToRenderGraph();
					pedestals.push_back(pedestal);
					drawables.emplace("Pedestal" + std::to_string(i), pedestal);

					AddPointLight({ pedestal->GetPosition().x, pedestal->GetPosition().y + 2.0f, pedestal->GetPosition().z }, 1.0f, { 1.0f, 1.0f, 1.0f });
				}
			}
		}
		else
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
	camera = Camera(PI_DIV4, (float)width / (float)height, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 5.0f, -10.0f });
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
	camera.Update();
	directionalLight.Update();

	for (auto& [name, drawable] : drawables)
		drawable->Update();

	for (auto& diamond : diamonds)
	{
		diamond->Update(camera);
		if (!player.IsCarryingObject())
			if (Event::LeftIsClicked() && !diamond->IsUsed() && camera.Intersects(diamond->GetBounds()))
				player.PickUpDiamond(diamond);
	}

	if (player.IsCarryingObject())
		for (auto& pedestal : pedestals)
			if (Event::LeftIsClicked() && !pedestal->IsUsed() && camera.Intersects(pedestal->GetBounds()))
			{
				player.PlaceDiamond(pedestal);
				AddParticleSystem(100, 0.1f, 3.0f, 1.0f, 1.2f, 5.0f, Vector2{ 0.04f, 0.04f }, Vector3{ pedestal->GetPosition().x, pedestal->GetPosition().y + 3.0f, pedestal->GetPosition().z }, EmitterType::SPHERE);

				if (player.DiamondsPlaced() == 4)
					AddParticleSystem(1000, 0.05f, 1.5f, 6.0f, 10.0f, 1.0f, Vector2{ 0.05f, 0.05f }, Vector3{ -11.5f, 15.0f, 18.0f }, EmitterType::CONE);
			}

	ShaderData::Update(camera, directionalLight, (UINT)pointLights.size(), pointLights.data());
}