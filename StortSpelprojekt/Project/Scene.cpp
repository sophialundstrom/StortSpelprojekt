#include "Scene.h"
#include "ShaderData.h"
#include "Event.h"

#include <filesystem>

std::vector<std::string> &Scene::GetObjectNames()
{
	return objectNames;
}

std::string Scene::AddDrawable(const std::string& name, std::shared_ptr<Drawable> drawable)
{
	UINT numInstances = 0;
	std::string finalName = name;

	for (auto& [name, drawable] : drawables)
		if (name.find(finalName) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
	{
		finalName = finalName + std::to_string(numInstances);
		drawable->SetName(finalName);
	}
		
	drawables[finalName] = drawable;
	objectNames.push_back(finalName);

	return finalName;
}

std::string Scene::AddModel(const std::string& file, const std::string path)
{
	UINT numInstances = 0;
	std::string fileName = file;

	for (auto& [name, drawable] : drawables)
		if (name.find(file) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
	{
		fileName = file + std::to_string(numInstances);
		drawables[fileName] = std::make_shared<Model>(fileName, *std::dynamic_pointer_cast<Model>(drawables[file]));
	}
	else
		drawables[fileName] = std::make_shared<Model>(file, fileName);
		
	objectNames.push_back(fileName);

	drawables[fileName]->SetName(fileName);

	return fileName;
}

void Scene::AddModel(const std::string& name, std::shared_ptr<Drawable> drawable)
{
	drawables[name] = drawable;
	objectNames.push_back(name);
	drawables[name]->SetName(name);
}

void Scene::AddBoundingVolume(const std::string& name, std::shared_ptr<Drawable> drawable)
{
	drawables[name] = drawable;
	objectNames.push_back(name);
	drawables[name]->SetName(name);
}

void Scene::AddFriendlyNPC(const std::string& file)
{
	UINT numInstances = 0;
	std::string fileName = file;

	for (auto& [name, drawable] : drawables)
		if (name.find(file) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
	{
		fileName = file + std::to_string(numInstances);
		drawables[fileName] = std::make_shared<FriendlyNPC>(*std::dynamic_pointer_cast<Model>(drawables[file]));
	}

	else
		drawables[fileName] = std::make_shared<FriendlyNPC>(fileName);

	objectNames.push_back(fileName);
}

void Scene::AddFriendlyNPC(const std::string& name, std::shared_ptr<Drawable> drawable)
{
	drawables[name] = drawable;
	objectNames.push_back(name);
}

void Scene::AddHostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player, CombatStyle combatStyle)
{
	UINT numInstances = 0;
	std::string fileName = file;

	for (auto& [name, drawable] : drawables)
		if (name.find(file) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
	{
		fileName = file + std::to_string(numInstances);
		//drawables[fileName] = std::make_shared<HostileNPC>(*std::dynamic_pointer_cast<Model>(drawables[file]), hostileArrows);
	}
	else
		drawables[fileName] = std::make_shared<HostileNPC>(fileName, hostileArrows, player, combatStyle);

	objectNames.push_back(fileName);
}

void Scene::AddHostileNPC(const std::string& name, std::shared_ptr<Drawable> drawable)
{
	drawables[name] = drawable;
	objectNames.push_back(name);
}

void Scene::AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type)
{
	std::string name = "ParticleSystem";
	UINT numInstances = 0;
	for (auto& [name, drawable] : drawables)
		if (name.find("ParticleSystem") != std::string::npos)
			numInstances++;

	if (numInstances > 0)
		name += std::to_string(numInstances);

	auto particleSystem = std::make_shared<ParticleSystem>(maxParticles, timeBetweenParticles, particlesLifetime, minVelocity, maxVelocity, size, particleExtents, position, type);
	drawables.emplace(name, particleSystem);
}

void Scene::AddParticleSystem(const std::string name, std::shared_ptr<ParticleSystem> particleSystem, Vector3 pos)
{
	std::string uniqueName = name;
	UINT numInstances = 0;
	for (auto& [name, drawable] : drawables)
		if (name.find(uniqueName) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
		uniqueName += std::to_string(numInstances);

	particleSystem.get()->SetPosition(pos);

	drawables.emplace(uniqueName, particleSystem);
}

void Scene::AddParticleSystem(const std::string name, std::shared_ptr<ParticleSystem> system)
{
	std::string uniqueName = name;
	UINT numInstances = 0;
	for (auto& [name, drawable] : drawables)
		if (name.find(uniqueName) != std::string::npos)
			numInstances++;

	if (numInstances > 0)
		uniqueName += std::to_string(numInstances);


	drawables.emplace(uniqueName, system);
}

void Scene::AddPointLight(Vector3 position, float range, Vector3 attenuation, Vector4 color)
{
	if (pointLights.size() < MAX_LIGHTS)
		pointLights.push_back(PointLight(range, attenuation, color, position));
}


void Scene::SetDirectionalLight(float range, float startAngle, int startDir)
{
	directionalLight = DirectionalLight(range, startAngle, startDir);
}

void Scene::SetCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed, Vector3 position, Vector3 forward, Vector3 up)
{
	camera = new Camera(FOV, aspectRatio, nearZ, farZ, rotationSpeed, moveSpeed, position, forward, up);
}

//OPEN FILE
Scene::Scene(const std::string& file)
{
	//TO DO: READ FROM FILE TO CREATE SCENE FROM STORED DATA
}

void Scene::Update()
{
	camera->Update();
	//directionalLight.Update();

	for (auto& [name, drawable] : drawables)
		drawable->Update();

	ShaderData::Inst().Update(*camera, directionalLight, (UINT)pointLights.size(), nullptr);
}