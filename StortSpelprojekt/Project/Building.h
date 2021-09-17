#pragma once
#include "Model.h"

class Building :public Model
{
private:
	std::string meshNames[3];
	std::string materialNames[3];
	UINT currState = 0;

public:
	Building(std::string meshNames[], std::string materialNames[], const std::string &fileName)
		:Model(fileName)
	{
		
	}

	void Upgrade()
	{
		currState++;
		ApplyMesh(meshNames[currState]);
		ApplyMaterial(materialNames[currState]);
	}

};