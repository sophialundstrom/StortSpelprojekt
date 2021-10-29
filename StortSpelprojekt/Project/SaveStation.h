#pragma once
#include "Model.h"
#include "Collision.h"

class SaveStation
{
private:
	struct Crystal : public Model
	{
		float minPos, maxPos;
		int direction;
		float speed;

		Crystal(const std::string& meshName, const std::string name) :Model(meshName, name), minPos(0), maxPos(0), direction(0), speed(0) {}
	};

	static const UINT numCrystals = 5;
	std::shared_ptr<Model> base;
	std::shared_ptr<Crystal> crystals[numCrystals];
	std::shared_ptr<BoundingBox> collider;

	float lastSave;
public:
	SaveStation() = default;
	SaveStation(Vector3 position, UINT ID, std::map<std::string, std::shared_ptr<Drawable>>& drawables)
		:lastSave(0)
	{
		base = std::make_shared<Model>("SaveStationBase", "SaveStationBase" + std::to_string(ID));
		base->SetPosition(position);
		base->SetRotation(0.0f, PI, 0.0f);
		drawables[base->GetName()] = base;
		base->Update();

		collider = std::make_shared<BoundingBox>();
		collider->SetScale(1.75f, 4.5f, 2.0f);
		collider->SetPosition(-0.5f, 4.0f, 0.0f);
		collider->SetParent(base);
		collider->Update();

		for (UINT i = 0; i < numCrystals; ++i)
		{
			crystals[i] = std::make_shared<Crystal>("SaveStationCrystal", "SaveStation" + std::to_string(ID) + "Crystal" + std::to_string(i));
			const float z = (int)i - 2;
			const float y = 8.0f - (float)pow((int)i - 2, 2) * 0.30f + 2.0f;
			crystals[i]->maxPos = y + 0.25f;
			crystals[i]->minPos = y - 0.25f;
			crystals[i]->SetPosition(0.0f, y, z);
			crystals[i]->SetParent(base);
			crystals[i]->speed = Random::Real(0.25f, 0.30f);
			crystals[i]->direction = (i % 2 == 0) ? -1 : 1;
			drawables[crystals[i]->GetName()] = crystals[i];
		}
	}

	std::shared_ptr<BoundingBox> Collider() { return collider; }

	void Update()
	{
		for (auto& crystal : crystals)
		{
			auto position = crystal->GetPosition();
			
			float x = ((position.y - crystal->minPos) / (crystal->maxPos - crystal->minPos)) * 2;

			float speed = -(double)pow(x, 2) + 2 * x;
			if (speed < 0 + 0.1 || speed > 2 - 0.1)
				speed += 0.1f;

			float offset = crystal->direction * Time::GetDelta() * crystal->speed * speed;

			if (position.y + offset > crystal->maxPos || position.y + offset < crystal->minPos)
			{
				crystal->direction *= -1;
				continue;
			}

			position = { position.x, position.y + offset, position.z };
			crystal->SetPosition(position);

			crystal->Update();
		}
	}

	void LastSave(float time) { lastSave = time; }
	float LastSave() { return lastSave; }
};