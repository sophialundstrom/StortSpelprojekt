#include "Scene.h"
#include "ShaderData.h"
#include "SettingsLoader.h"
#include "Event.h"

#include <filesystem>

void Scene::AddModel(const std::string& file)
{
	UINT numInstances = 0;
	std::string fileName = file;

	for (auto& [name, drawable] : drawables)
		if (name.find(file) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
	{
		fileName = file + std::to_string(numInstances);
		drawables[fileName] = std::make_shared<TempModel>(*std::dynamic_pointer_cast<TempModel>(drawables[file]));
	}

	else
		drawables[fileName] = std::make_shared<TempModel>(fileName);
}

void Scene::AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type)
{
	std::string name = "Particle System";
	UINT numInstances = 0;
	for (auto& [name, drawable] : drawables)
		if (name.find("Particle System") != std::string::npos)
			numInstances++;

	if (numInstances > 0)
		name += std::to_string(numInstances);

	auto particleSystem = std::make_shared<ParticleSystem>(maxParticles, timeBetweenParticles, particlesLifetime, minVelocity, maxVelocity, size, particleExtents, position, type);
	particleSystem->BindToRenderGraph();
	drawables.emplace(name, particleSystem);
}

void Scene::AddPointLight(Vector3 position, float range, Vector3 attenuation, Vector4 color)
{
	if (pointLights.size() < MAX_LIGHTS)
		pointLights.push_back(PointLight(position, range, attenuation, color));
}

void Scene::SetDirectionalLight(float range, float startAngle, int startDir)
{
	directionalLight = DirectionalLight(range, startAngle, startDir);
}

void Scene::SetCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed, Vector3 position, Vector3 forward, Vector3 up)
{
	camera = Camera(FOV, aspectRatio, nearZ, farZ, rotationSpeed, moveSpeed, position, forward, up);
}

//SAVE FILE (EITHER DEFAULT OR SAVED GAME)
void Scene::SaveFile(const std::string& file)
{
	//TO DO: CONVERT STORED DRAWABLE DATA TO READABLE FILE FORMAT
}

//OPEN FILE (EITHER DEFAULT OR SAVED GAME)
Scene::Scene(const std::string& file)
{
	//TO DO: READ FROM FILE TO CREATE SCENE FROM STORED DATA
}

void Scene::Update()
{
	camera.Update();
	directionalLight.Update();

	for (auto& [name, drawable] : drawables)
		drawable->Update();

	ShaderData::Inst().Update(camera, directionalLight, (UINT)pointLights.size(), pointLights.data());
}