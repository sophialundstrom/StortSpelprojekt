#pragma once
#include "Model.h"

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
public:
	SaveStation() = default;
	SaveStation(Vector3 position, UINT ID, std::map<std::string, std::shared_ptr<Drawable>>& drawables)
	{
		base = std::make_shared<Model>("SaveStationBase", "SaveStationBase" + std::to_string(ID));
		base->SetPosition(position);
		base->SetRotation(0, PI, 0);
		drawables[base->GetName()] = base;

		for (UINT i = 0; i < numCrystals; ++i)
		{
			crystals[i] = std::make_shared<Crystal>("SaveStationCrystal", "SaveStation" + std::to_string(ID) + "Crystal" + std::to_string(i));
			const float z = (int)i - 2;
			const float y = 8 - pow((int)i - 2, 2) * 0.30f + 2;
			crystals[i]->maxPos = y + 0.25;
			crystals[i]->minPos = y - 0.25;
			crystals[i]->SetPosition(0, y, z);
			crystals[i]->SetParent(base);
			crystals[i]->speed = Random::Real(0.25, 0.30);
			crystals[i]->direction = (i % 2 == 0) ? -1 : 1;
			drawables[crystals[i]->GetName()] = crystals[i];
		}
	}

	void Update()
	{
		for (auto& crystal : crystals)
		{
			auto position = crystal->GetPosition();
			
			float offset = crystal->direction * Time::GetDelta() * crystal->speed;

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

	void Interact()
	{

	}
};