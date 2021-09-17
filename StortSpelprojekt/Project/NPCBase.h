#pragma once
#include "Model.h"
#include "Scene.h"

class NPC
{
protected:
	NPC(const std::string& file);
	//Transform t;
	std::shared_ptr<Model> model;
	void AddModel(Scene* scene, const std::string& file);
};