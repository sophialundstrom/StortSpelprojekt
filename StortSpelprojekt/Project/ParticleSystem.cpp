#include "ParticleSystem.h"

#include "RenderGraph.h"

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

void ParticleSystem::BindToRenderGraph()
{
	RenderGraph::Bind(shared_from_this(), RendererType::PARTICLE);
}

Vector2 ParticleSystem::GetParticleExtents() const
{
	return this->particleExtents;
}

void ParticleSystem::BindBuffer()
{
	Graphics::GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

void ParticleSystem::DrawParticles()
{
	Graphics::GetContext().Draw(particleCount, 0);
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