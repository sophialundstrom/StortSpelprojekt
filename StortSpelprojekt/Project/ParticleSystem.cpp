#include "ParticleSystem.h"

#include "FileSystem.h"
#include "Time.h"
#include <fstream>

std::string GetNthString(const std::string& line, UINT n)
{
	std::string temp = line;
	for (UINT i = 0; i < (n * 2 - 1); ++i)
	{
		size_t location = temp.find_first_of(39); // " ' " ascii
		temp = temp.substr(location + 1, temp.length());
	}

	return temp.substr(0, temp.find_first_of(39));
}

ParticleSystem::ParticleSystem(const std::string& file, bool preview)
	:maxParticles(0), timeBetweenParticles(0), particlesLifetime(0), minVelocity(0), maxVelocity(0), size(0), particleExtents(0, 0), type(EmitterType::SPHERE),
	timeSinceLastParticle(0), particleCount(0)
{
	std::string path = file;
	std::string line = "";

	this->position = { 0.0f, 0.0f, 0.0f };
	this->particleDirection = true;

	if (file == "default.ps" || file.find("\\") == std::string::npos)
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
		reader >> width;
		reader >> depth;

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

		std::getline(reader, line);
	}

	std::string firstImageFile = GetNthString(line, 1);
	std::string firstImagePath = FileSystem::ProjectDirectory::path + "\\ParticleTextures\\" + firstImageFile;
	firstTexture = new Texture(firstImagePath, firstImageFile);

	std::string secondImageFile = GetNthString(line, 2);
	std::string secondImagePath = FileSystem::ProjectDirectory::path + "\\ParticleTextures\\" + secondImageFile;
	secondTexture = new Texture(secondImagePath, secondImageFile);

	std::string opacityImageFile = GetNthString(line, 3);
	std::string opacityImagePath = FileSystem::ProjectDirectory::path + "\\ParticleTextures\\" + opacityImageFile;
	opacityTexture = new Texture(opacityImagePath, opacityImageFile);


	if (preview)
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * ABSOLUTE_MAX_PARTICLES);
	else
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * maxParticles);
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, float timeBetweenParticles, float particlesLifetime, float minVelocity, float maxVelocity, float size, Vector2 particleExtents, Vector3 position, EmitterType type)
	:maxParticles(maxParticles), size(size), maxVelocity(maxVelocity), minVelocity(minVelocity), particleExtents(particleExtents),
	timeBetweenParticles(timeBetweenParticles), particlesLifetime(particlesLifetime), type(type), timeSinceLastParticle(0), particleCount(0)
{
	this->position = { 0.0f, 0.0f, 0.0f };
	CreateDynamicVertexBuffer(vertexBuffer, sizeof(Particle), sizeof(Particle) * maxParticles);
}

ParticleSystem::~ParticleSystem()
{
	vertexBuffer->Release();
	delete firstTexture;
	delete secondTexture;
}

void ParticleSystem::Draw() const
{
	firstTexture->Bind();
	secondTexture->Bind(1);
	opacityTexture->Bind(2);
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
	if (particleCount < maxParticles && stopSpawn == false)
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
				if (particleDirection)
				{
					newParticle.direction = { 0.0f, 1.0f, 0.0f };
					//newParticle.color = { 55.0f / 255.0f, 71.0f / 255.0f, 75.0f / 255.0f };

					newParticle.position.x = Random::Real(position.x - width / 2.0f, position.x + width / 2.0f);
					newParticle.position.z = Random::Real(position.z - depth / 2.0f, position.z + depth / 2.0f);
					newParticle.position.y = position.y;
				}
				else if (!particleDirection)
				{
					newParticle.direction = { 0.0f, -1.0f, 0.0f };
					//newParticle.color = { 55.0f / 255.0f, 71.0f / 255.0f, 75.0f / 255.0f };

					newParticle.position.x = Random::Real(position.x - width / 2.0f, position.x + width / 2.0f);
					newParticle.position.z = Random::Real(position.z - depth / 2.0f, position.z + depth / 2.0f);
					newParticle.position.y = position.y;
				}

					

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

				//newParticle.color = colors[Random::Integer(0, ARRAYSIZE(colors) - 1)];
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

				//newParticle.color = colors[Random::Integer(0, ARRAYSIZE(colors) - 1)];
				break;
			}
			}

			newParticle.velocity = Random::Real(minVelocity, maxVelocity);
			newParticle.useAlpha = useAlpha;
			newParticle.useOpacity = useOpacity;

			if (scaling)
			{
				newParticle.scalingOverTime = scaleOverTime;
			}

			if (rotating)
			{
				newParticle.rotationSpeed = Random::Real(minRotationSpeed, maxRotationSpeed);
				auto rotDir = Random::Integer(0,1);
				if (rotDir == 0)
					newParticle.rotationDir = -1;
				else
					newParticle.rotationDir = 1;
			}
			
			particles.push_back(newParticle);
			particleCount++;
		}
	}
	UINT index = 0;
	//UPDATE EXISTING PARTICLES
	for (auto& particle : particles)
	{
		index++;
		//if (type == EmitterType::CONE)
		//{
		//	if (particle.lifeTime > particlesLifetime * 0.5f)
		//	{
		//		float yDir = particle.direction.y;
		//		particle.direction *= sin(particle.lifeTime);
		//		particle.direction.y = yDir;
		//		//particle.direction += Vector3(0.0f, 1.0f, 0.0f);
		//		particle.direction.Normalize();
		//	}
		//}
		
		particle.position += particle.direction * Time::GetDelta() * particle.velocity;
		particle.lifeTime += Time::GetDelta();

		if (particle.lifeTime > particlesLifetime)
		{
			if (stopSpawn == true)
			{
				particles.erase(particles.begin() + index - 1);
				particleCount--;
				index--;
				continue;
			}
			if (type == EmitterType::CUBE)
				particle.position.y = position.y;
			else
				particle.position = position;

			particle.lifeTime = 0.0f;
		}
	}
	if (particles.empty())
		done = true;
	//LASTLY UPDATE BUFFER
	UpdateBuffer(vertexBuffer, particles.data(), sizeof(Particle) * particleCount);
}

void ParticleSystem::ChangeSecondTexture(std::string path, std::string fileName)
{
	if (secondTexture)
		delete secondTexture;

	secondTexture = new Texture(path, fileName);
}

void ParticleSystem::ChangeFirstTexture(std::string path, std::string fileName)
{
	if (firstTexture)
		delete firstTexture;

	firstTexture = new Texture(path, fileName);
}

void ParticleSystem::ChangeOpacityTexture(std::string path, std::string fileName)
{
	if (opacityTexture)
		delete opacityTexture;

	opacityTexture = new Texture(path, fileName);
}
