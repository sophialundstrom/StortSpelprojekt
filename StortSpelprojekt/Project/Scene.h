#pragma once
#include "Light.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "TempModel.h"

#include <map>

struct SceneComponents
{

};

class Scene
{
private:
	Camera camera;
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::map<std::string, std::shared_ptr<Drawable>> drawables;
public:
	Scene(const std::string& file);
	Scene() = default;

	void Update();

	Camera& GetCamera() { return this->camera; }

	template <typename T>
	auto Get(const std::string& name) { return std::dynamic_pointer_cast<T>(drawables[name]); }

	void AddModel(const std::string& file);
	void AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	void AddPointLight(Vector3 position, float range, Vector3 attenuation = { 0.05f, 0.05f, 0.05f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
	void SetDirectionalLight(float range, float startAngle = 0.0f, int startDir = 1);
	void SetCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 forward = { 0.0f, 0.0f, 1.0f }, Vector3 up = { 0.0f, 1.0f, 0.0f });
	
	void SaveFile(const std::string& file);
};