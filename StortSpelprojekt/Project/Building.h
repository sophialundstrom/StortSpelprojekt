#pragma once
#include "Model.h"

class Building :public Model
{
private:
	static const UINT stages = 2;
	std::string meshNames[stages];
	std::string materialNames[stages];
	UINT currState = 0;

public:
	Building() = default;

	Building(std::string meshNames[], std::string materialNames[], const std::string &fileName)
		:Model(fileName)
	{
		for (UINT i = 0; i < stages; i++)
		{
			this->meshNames[i] = meshNames[i];
			this->materialNames[i] = materialNames[i];
		}
	}

	void Upgrade()
	{
		currState++;
		if (currState >= stages)
			return;
		ApplyMesh(meshNames[currState]);
		ApplyMaterial(materialNames[currState]);
	}

};