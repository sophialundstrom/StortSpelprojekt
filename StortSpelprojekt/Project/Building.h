#pragma once
#include "Model.h"

class Building :public Model
{
private:
	static const UINT stages = 3;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 0;
	float lastUpdate = 0;
public:
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &fileName)
		:Model(fileName)
	{
		for (UINT i = 0; i < stages; i++)
		{
			this->meshNames[i] = meshNames[i];
			this->materialNames[i] = materialNames[i];
			Model init = Model(meshNames[i]);
		}
	}

	void Upgrade()
	{
		if (Time::Get() - lastUpdate < 3.0f)
			return;

		lastUpdate = Time::Get();

		if (currState >= stages)
			return;

		currState++;

		ApplyMesh(meshNames[currState - 1]);
		ApplyMaterial(materialNames[currState - 1]);
	}

};