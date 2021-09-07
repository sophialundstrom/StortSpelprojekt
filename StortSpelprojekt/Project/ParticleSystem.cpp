#include "ParticleSystem.h"

#include "RenderGraph.h"
#include "FileSystem.h"
#include "Time.h"
#include <fstream>

ParticleSystem::ParticleSystem(const std::string& file, bool preview)
	:maxParticles(0), timeBetweenParticles(0), particlesLifetime(0), minVelocity(0), maxVelocity(0), size(0), particleExtents(0,0), position(0,0,0), type(EmitterType::SPHERE), 
	timeSinceLastParticle(0), particleCount(0)
{
	std::string path = file;

	if (file == "default.ps")
		path = FileSystem::ProjectDirectory::path + "\\ParticleSystems\\" + file;

	std::ifstream reader;

	reader.open(path, std::ios::beg);
	if (!reader.is_open())
	{
		Print("FAILED TO READ PARTICLE SYSTEM FILE");
		return;
	}

	while (!reader.eof())
	{
		reader >> maxParticles;
		reader >> timeBetweenParticles;
		reader >> particlesLifetime;
		reader >> size;

		reader >> minVelocity;
		reader >> maxVelocity;

		reader >> particleExtents.x;
		reader >> particleExtents.y;

		UINT temp;
		reader >> temp;
		type = EmitterType(temp);

		reader >> position.x;
		reader >> position.y;
		reader >> position.z;
	}

	if (preview)
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * ABSOLUTE_MAX_PARTICLES);
	else
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * maxParticles);
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type)
	:maxParticles(maxParticles), size(size), position(position), maxVelocity(maxVelocity), minVelocity(minVelocity), particleExtents(particleExtents),
	timeBetweenParticles(timeBetweenParticles), particlesLifetime(particlesLifetime), type(type), timeSinceLastParticle(0), particleCount(0)
{
	CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * maxParticles);
}

ParticleSystem::~ParticleSystem()
{
	vertexBuffer->Release();
}

void ParticleSystem::Draw() const
{
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	Graphics::Inst().GetContext().Draw(particleCount, 0);
}

void ParticleSystem::Reset()
{
	particles.clear();
	timeSinceLastParticle = 0;
	particleCount = 0;
}

void ParticleSystem::Update()
{
	//IF THERE IS SPACE FOR MORE PARTICLES
	if (particleCount < maxParticles)
	{
		timeSinceLastParticle += Time::GetDelta();

		if (timeSinceLastParticle >= timeBetweenParticles)
		{
			timeSinceLastParticle = 0;

			//CREATE NEW PARTICLE ACCORDING TO TYPE AND ADD TO LIST
			Particle newParticle = {};

			switch (type)
			{

			case EmitterType::CUBE:
			{
				newParticle.direction = { 0.0f, -1.0f, 0.0f };
				newParticle.color = { 55.0f / 255.0f, 71.0f / 255.0f, 75.0f / 255.0f };

				newParticle.position.x = Random::Real(position.x - size / 2.0f, position.x + size / 2.0f);
				newParticle.position.z = Random::Real(position.z - size / 2.0f, position.z + size / 2.0f);
				newParticle.position.y = position.y;
				break;
			}

			case EmitterType::SPHERE:
			{
				newParticle.position = position;

				float phi = Random::Real(0, PI * 2);
				float theta = Random::Real(0, PI);

				float x = sin(theta) * cos(phi);
				float y = sin(theta) * sin(phi);
				float z = cos(theta);

				Vector3 direction = Vector3(x, y, z);
				direction.Normalize();

				newParticle.direction = direction;

				newParticle.color = colors[Random::Integer(0, ARRAYSIZE(colors) - 1)];
				break;
			}

			case EmitterType::CONE:
			{
				newParticle.position = position;

				float phi = Random::Real(0, PI * 2);
				float theta = Random::Real(0, PI / 6.0f);

				float x = sin(theta) * cos(phi) * size;
				float z = sin(theta) * sin(phi) * -1.0f * size;
				float y = cos(theta);

				Vector3 direction = Vector3(x, y, z);
				direction.Normalize();

				newParticle.direction = direction;

				newParticle.color = colors[Random::Integer(0, ARRAYSIZE(colors) - 1)];
				break;
			}
			}

			newParticle.velocity = Random::Real(minVelocity, maxVelocity);

			particles.push_back(newParticle);
			particleCount++;
		}
	}

	//UPDATE EXISTING PARTICLES
	for (auto& particle : particles)
	{
		particle.position += particle.direction * Time::GetDelta() * particle.velocity;
		particle.lifeTime += Time::GetDelta();

		if (particle.lifeTime > particlesLifetime)
		{
			if (type == EmitterType::CUBE)
				particle.position.y = position.y;
			else
				particle.position = position;

			particle.lifeTime = 0.0f;
		}
	}

	//LASTLY UPDATE BUFFER
	UpdateBuffer(vertexBuffer, particles.data(), sizeof(Particle) * particleCount);
}