#pragma once
#include "Drawable.h"
#include "DirectXHelp.h"
#include "Math.h"
#include "Random.h"

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
		Vector3 color;
		float lifeTime;
		float velocity;
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
	Vector3 position;

	Vector2 particleExtents;

	float minVelocity;
	float maxVelocity;

	unsigned int particleCount;
	unsigned int maxParticles;

	float particlesLifetime;

	float timeBetweenParticles;
	float timeSinceLastParticle;

	std::vector<Particle> particles;
	ID3D11Buffer* vertexBuffer = nullptr;
public:
	static const UINT ABSOLUTE_MAX_PARTICLES = 1000000u;

	ParticleSystem() = default;
	ParticleSystem(const std::string& file, bool preview = false);
	ParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	~ParticleSystem();

	void Draw() const;

	void Reset();
	void Update();

	//SET
	void SetType(const EmitterType& type)				{ this->type = type; }
	void SetSize(float size)							{ this->size = size; }
	void SetParticleExtents(Vector2 extents)			{ this->particleExtents = extents; }
	void SetParticleExtents(float width, float height)	{ this->particleExtents = Vector2(width, height); }
	void SetParticleWidth(float value)					{ this->particleExtents.x = value; }
	void SetParticleHeight(float value)					{ this->particleExtents.y = value; }
	void SetMinVelocity(float value)					{ this->minVelocity = value; }
	void SetMaxVelocity(float value)					{ this->maxVelocity = value; }
	void SetMaxParticles(unsigned int amount)			{ this->maxParticles = amount; }
	void SetParticlesLifetime(float amount)				{ this->particlesLifetime = amount; }
	void SetTimeBetweenPartilces(float amount)			{ this->timeBetweenParticles = amount; }
	
	Vector2 GetParticleExtents() const		{ return this->particleExtents; }
	EmitterType GetType() const				{ return this->type; }
	float GetSize() const						{ return this->size; }
	float GetParticleWidth() const			{ return this->particleExtents.x; }
	float GetParticleHeight() const			{ return this->particleExtents.y; }
	float GetMinVelocity() const			{ return this->minVelocity; }
	float GetMaxVelocity() const			{ return this->maxVelocity; }
	unsigned int GetMaxParticles() const	{ return this->maxParticles; }
	float GetParticlesLifetime() const		{ return this->particlesLifetime; }
	float GetTimeBetweenParticles() const	{ return this->timeBetweenParticles; }
};
