#pragma once
#include "Drawable.h"
#include "DirectXHelp.h"
#include "Math.h"
#include "Random.h"
#include "Texture.h"

enum class EmitterType { SPHERE, CUBE, CONE };

class ParticleSystem : public Drawable, public std::enable_shared_from_this<ParticleSystem>
{
private:
	const UINT stride = sizeof(Particle);
	const UINT offset = 0;

	struct Particle
	{
		Vector3 position;
		Vector3 direction;
		float lifeTime;
		float velocity;
		int rotationDir;
		float rotationSpeed;
		int useAlpha;
		int useOpacity;
	};

	const Vector3 colors[5] =
	{
		{ 97.0f / 255.0f, 127.0f / 255.0f, 134.0f / 255.0f },
		{ 103.0f / 255.0f, 155.0f / 255.0f, 168.0f / 255.0f },
		{ 22.0f / 255.0f, 44.0f / 255.0f, 49.0f / 255.0f },
		{ 67.0f / 255.0f, 120.0f / 255.0f, 133.0f / 255.0f },
		{ 55.0f / 255.0f, 71.0f / 255.0f, 75.0f / 255.0f }
	};
private:
	EmitterType type;

	float size;
	float width;
	float depth;

	Vector2 particleExtents;

	float minVelocity;
	float maxVelocity;

	int useAlpha = 1;
	int useOpacity = 1;

	unsigned int particleCount;
	unsigned int maxParticles;

	float particlesLifetime;

	float timeBetweenParticles;
	float timeSinceLastParticle;

	bool rotating = false;

	std::vector<Particle> particles;
	ID3D11Buffer* vertexBuffer = nullptr;
	Texture* firstTexture;
	Texture* secondTexture;
	Texture* opacityTexture;

	bool particleDirection = false;
	bool done = false;
	bool stopSpawn = false;

public:
	static const UINT ABSOLUTE_MAX_PARTICLES = 1000000u;

	ParticleSystem() = default;
	ParticleSystem(const std::string& file, bool preview = false);
	ParticleSystem(const std::string& file, Vector3 pos);
	ParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	~ParticleSystem();

	void Draw() const;

	float minRotationSpeed = 0.0f;
	float maxRotationSpeed = 0.0f;

	void Reset();
	void Update();

	//SET
	void SetType(const EmitterType& type) { this->type = type; }
	void SetSize(float size) { this->size = size; }
	void SetParticleExtents(Vector2 extents) { this->particleExtents = extents; }
	void SetParticleExtents(float width, float height) { this->particleExtents = Vector2(width, height); }
	void SetParticleWidth(float value) { this->particleExtents.x = value; }
	void SetParticleHeight(float value) { this->particleExtents.y = value; }
	void SetMinVelocity(float value) { this->minVelocity = value; }
	void SetMaxVelocity(float value) { this->maxVelocity = value; }
	void SetMaxParticles(unsigned int amount) { this->maxParticles = amount; }
	void SetParticlesLifetime(float amount) { this->particlesLifetime = amount; }
	void SetTimeBetweenPartilces(float amount) { this->timeBetweenParticles = amount; }
	void SetPosition(Vector3 pos) { this->position = pos; }

	bool GetParticleDir() { return this->particleDirection; }
	void SetParticleDir(bool value) { particleDirection = value; }

	void SetXPosition(float xPos) { this->position.x = xPos; }
	void SetYPosition(float yPos) { this->position.y = yPos; }
	void SetZPosition(float zPos) { this->position.z = zPos; }

	void SetMinRotationSpeed(float speed) { this->minRotationSpeed = speed; }
	void SetMaxRotationSpeed(float speed) { this->maxRotationSpeed = speed; }
	float GetMinRotation() { return this->minRotationSpeed; }
	float GetMaxRotation() { return this->maxRotationSpeed; }
	void SetRotation(bool rotating) { this->rotating = rotating; }
	bool GetRotation() { return this->rotating; }

	void SetAlphaMode(int mode) { this->useAlpha = mode; }
	int GetAlphaMode() { return this->useAlpha; }
	void SetOpacityMode(int mode) { this->useOpacity = mode; }
	int GetOpacityMode() { return this->useOpacity; }

	void StartSpawn() { this->stopSpawn = false; this->done = false; }
	void StopSpawn() { this->stopSpawn = true; }
	bool IsDone() { return this->done; }
	void ChangeFirstTexture(std::string path, std::string fileName);
	void ChangeSecondTexture(std::string path, std::string fileName);
	void ChangeOpacityTexture(std::string path, std::string fileName);

	void SetCubeWidth(float width) { this->width = width; }
	void SetCubeDepth(float depth) { this->depth = depth; }


	ID3D11ShaderResourceView* GetFirstTexture() { return firstTexture->Get(); }
	std::string GetFirstTexturePath() { return firstTexture->GetPath(); }
	std::string GetFirstTextureFile() { return firstTexture->GetFile(); }

	ID3D11ShaderResourceView* GetSecondTexture() { return secondTexture->Get(); }
	std::string GetSecondTexturePath() { return secondTexture->GetPath(); }
	std::string GetSecondTextureFile() { return secondTexture->GetFile(); }

	//Opacity
	ID3D11ShaderResourceView* GetOpacityTexture() { return opacityTexture->Get(); }
	std::string GetOpacityTexturePath() { return opacityTexture->GetPath(); }
	std::string GetOpacityTextureFile() { return opacityTexture->GetFile(); }

	Vector2 GetParticleExtents() const { return this->particleExtents; }
	EmitterType GetType() const { return this->type; }
	float GetSize() const { return this->size; }
	float GetWidth() const { return this->width; }
	float GetDepth() const { return this->depth; }
	float GetParticleWidth() const { return this->particleExtents.x; }
	float GetParticleHeight() const { return this->particleExtents.y; }
	float GetMinVelocity() const { return this->minVelocity; }
	float GetMaxVelocity() const { return this->maxVelocity; }
	unsigned int GetMaxParticles() const { return this->maxParticles; }
	float GetParticlesLifetime() const { return this->particlesLifetime; }
	float GetTimeBetweenParticles() const { return this->timeBetweenParticles; }
};