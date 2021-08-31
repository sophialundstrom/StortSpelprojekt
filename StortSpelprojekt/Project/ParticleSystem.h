#pragma once
#include "Drawable.h"
#include "DirectXHelp.h"
#include "Math.h"
#include "Random.h"

enum class EmitterType { CUBE, SPHERE, CONE };

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
	ID3D11Buffer* vertexBuffer;
public:
	ParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type);
	~ParticleSystem();

	void BindToRenderGraph();

	Vector2 GetParticleExtents() const;

	void BindBuffer();
	void DrawParticles();

	void Update();
};
