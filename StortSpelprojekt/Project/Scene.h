#pragma once
#include "Light.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "Model.h"
#include "AnimatedModel.h"
#include <map>

class Scene
{
private:
	Camera* camera = nullptr;
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::map<std::string, std::shared_ptr<Drawable>> drawables;
	std::vector<std::string> objectNames;
public:
	Scene(const std::string& file);
	Scene() = default;
	~Scene() { delete camera; }

	std::map<std::string, std::shared_ptr<Drawable>>& GetDrawables() { return drawables; }

	void Clear() { drawables.clear(); pointLights.clear(); }
	void Update();

	Camera* GetCamera() { return this->camera; }

	//GET DRAWABLE BY SPECIFYING TYPE AND NAME
	template <typename T>
	auto Get(const std::string& name) { return std::dynamic_pointer_cast<T>(drawables[name]); }

	std::vector<std::string>& GetObjectNames();

	std::string AddDrawable(const std::string& name, std::shared_ptr<Drawable> drawable);
	std::string AddDrawable(std::shared_ptr<Drawable> drawable);
	std::string AddModel(const std::string& file, const std::string path);
	std::string AddModel(std::string fileName, const std::string& file, const std::string path);
	void AddModel(const std::string& name,std::shared_ptr <Drawable> drawable);
	void AddBoundingVolume(const std::string& name, std::shared_ptr <Drawable> drawable);

	void DeleteDrawable(const std::string name) { drawables.erase(name); }

	//void AddFriendlyNPC(const std::string& file);
	//void AddFriendlyNPC(const std::string& name, std::shared_ptr <Drawable> drawable);

	//void AddHostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player);
	//void AddHostileNPC(const std::string& name, std::shared_ptr<Drawable> drawable);

	DirectionalLight GetDirectionalLight() { return directionalLight; }

	void AddAnimatedModel(const std::string& file);

	void AddParticleSystem(const std::string name, std::shared_ptr<ParticleSystem> system, Vector3 pos);
	void AddParticleSystem(const std::string name, std::shared_ptr<ParticleSystem> system);
	void AddParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	
	void AddPointLight(Vector3 position, float range, Vector3 attenuation = { 0.05f, 0.05f, 0.05f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	PointLight* GetPointLights() { return pointLights.data(); }
	UINT GetNumberOfPointlights() { return (UINT)pointLights.size(); }
	void UpdatePointLights();

	void UpdateDirectionalLight(const Vector3& position) { directionalLight.SetTargetPosition(position); }
	void UpdateDirectionalLightColor(const Vector4 color) { directionalLight.data.color = color; }
	void SetDirectionalLight(float range, Vector4 color = { 1, 1, 1, 1 }, float startAngle = 0.0f, int startDir = 1);
	void SetCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 forward = { 0.0f, 0.0f, 1.0f }, Vector3 up = { 0.0f, 1.0f, 0.0f });
};